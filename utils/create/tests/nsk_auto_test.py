#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import subprocess
import shutil
import textwrap
import filecmp

from pathlib import Path
from dataclasses import dataclass
from typing import Iterable, List, Callable

PROGRAM_COMPILER     = "ca65"
PROGRAM_LINKER       = "ld65"
PROGRAM_MAKE         = "make"

DIR_BIN              = "bin"
DIR_BUILD            = "build"
DIR_CASES            = "cases"
DIR_HELPERS          = "subroutines"
DIR_MEMORY           = "memory"
DIR_READER           = "utils/head"

CONFIG_FILE_CONSTS   = "nsk_header_consts.inc"
CONFIG_FILE_SOURCE   = "nsk_header_code.asm"
CONFIG_FILE_MEMORY   = "nsk_header_memory.cfg"

TEST_FILE_OBJECT     = "test.o"
TEST_FILE_BINARY     = "test.nes"
TEST_FILE_CONFIG     = "nsk_header_config.inc"
TEST_FILE_REFERENCE  = "rom-header.nes"
TEST_FILE_XML        = "nes20db-slice.xml"

HELPER_CONFIG_CREATE = "nsk_config_create.py"
HELPER_ROM_READER    = "nesokia-head"

ANSI_COLOR_RED       = 91
ANSI_COLOR_GREEN     = 92
ANSI_COLOR_YELLOW    = 93

@dataclass
class BuildSetup:
    @dataclass
    class BuildCompiler:
        ca65: Path
        ld65: Path
        make: Path

    @dataclass
    class BuildDirectories:
        cases  : Path
        helpers: Path
        reader : Path

    @dataclass
    class BuildHelpers:
        generator: Path
        reader: Path

    @dataclass
    class BuildConfig:
        consts: Path
        source: Path
        memory: Path

    compiler:  BuildCompiler
    directory: BuildDirectories
    helpers:   BuildHelpers
    config:    BuildConfig

@dataclass
class CaseSetup:
    root      : Path
    slice     : Path
    object    : Path
    target    : Path
    config    : Path
    reference : Path
    failed    : bool

def color(text: str, code: int) -> str:
    return f"\033[{str(code)}m{text}\033[0m"

def build_setup() -> BuildSetup:
    here = Path(__file__).resolve().parent

    dir_cases = (here / DIR_CASES).resolve()
    if not dir_cases.is_dir():
        raise RuntimeError(f"Cases dir not found: {dir_cases}")
    dir_helpers = (here / DIR_HELPERS).resolve()
    if not dir_helpers.is_dir():
        raise RuntimeError(f"Helpers dir not found: {dir_helpers}")
    dir_reader = (here.parent.parent.parent / DIR_READER).resolve()
    if not dir_reader.is_dir():
        raise RuntimeError(f"Reader dir not found: {dir_reader}")

    helper_generator = (dir_helpers / HELPER_CONFIG_CREATE).resolve()
    if not helper_generator.is_file():
        raise RuntimeError(f"Headers generator not found: {helper_generator}")
    helper_reader = (here.parent.parent.parent / DIR_BIN / HELPER_ROM_READER)
    if not helper_generator.is_file():
        raise RuntimeError(f"ROM reader not found: {helper_reader}")

    config_consts = (here.parent / CONFIG_FILE_CONSTS).resolve()
    if not config_consts.is_file():
        raise RuntimeError(f"Consts list file not found: {config_consts}")
    config_source = (here.parent / CONFIG_FILE_SOURCE).resolve()
    if not config_source.is_file():
        raise RuntimeError(f"Source file not found: {config_source}")
    config_memory = (here / DIR_MEMORY / CONFIG_FILE_MEMORY).resolve()
    if not config_memory.is_file():
        raise RuntimeError(f"Memory config not found: {config_memory}")

    compiler_ca65 = shutil.which(PROGRAM_COMPILER)
    if compiler_ca65 is None:
        raise RuntimeError(f"{PROGRAM_COMPILER} not found in PATH")
    compiler_ld65 = shutil.which(PROGRAM_LINKER)
    if compiler_ld65 is None:
        raise RuntimeError(f"{PROGRAM_LINKER} not found in PATH")
    compiler_make = shutil.which(PROGRAM_MAKE)
    if compiler_make is None:
        raise RuntimeError(f"{PROGRAM_MAKE} not found in PATH")

    return BuildSetup(
        compiler = BuildSetup.BuildCompiler(
            ca65 = Path(compiler_ca65),
            ld65 = Path(compiler_ld65),
            make = Path(compiler_make)
        ),
        directory = BuildSetup.BuildDirectories(
            cases   = dir_cases,
            helpers = dir_helpers,
            reader  = dir_reader
        ),
        helpers = BuildSetup.BuildHelpers(
            generator = helper_generator,
            reader    = helper_reader
        ),
        config = BuildSetup.BuildConfig(
            consts = config_consts,
            source = config_source,
            memory = config_memory
        )
    )

def print_passed(string: str):
    print(f"[{color('OK', ANSI_COLOR_GREEN)}]    {string}")

def print_failed(string: str):
    print(f"[{color('ERR', ANSI_COLOR_GREEN)}]   {string}")

def test_passed(case: CaseSetup):
    print_passed(case.root.name)

def test_failed(case: CaseSetup):
    print_failed(case.root.name)
    case.failed = True

def print_test_output(output: str):
    print(f"{textwrap.indent(output, '    ')}")

def print_rom_info(setup: BuildSetup, file: Path):
    cmd = [
        str(setup.helpers.reader),
        str(file)
    ]
    proc = subprocess.run(
        cmd,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT
    )
    print_test_output(proc.stdout)

def cases_setup(setup: BuildSetup) -> List[CaseSetup]:
    cases: List[CaseSetup] = []
    for sub in sorted(setup.directory.cases.iterdir()):
        if not sub.is_dir():
            continue

        file_slice = (sub / TEST_FILE_XML).resolve()
        if not file_slice.is_file():
            raise RuntimeError(f"{sub} does not contain {TEST_FILE_XML}")

        file_ref   = (sub / TEST_FILE_REFERENCE).resolve()
        if not file_ref.is_file():
            raise RuntimeError(f"{sub} does not contain {TEST_FILE_REFERENCE}")

        cases.append(
            CaseSetup(
                root        = sub,
                slice       = file_slice,
                object      = (sub / TEST_FILE_OBJECT).resolve(),
                target      = (sub / TEST_FILE_BINARY).resolve(),
                config      = (sub / TEST_FILE_CONFIG).resolve(),
                reference   = file_ref,
                failed      = False
            )
        )
    return cases

def test_directory_clean(setup: BuildSetup, cases: List[CaseSetup]) -> None:
    print("# Clearing old files")

    for case in cases:
        if case.failed:
            test_failed(case)
            continue

        case.object.unlink(missing_ok=True)
        case.target.unlink(missing_ok=True)
        case.config.unlink(missing_ok=True)
        test_passed(case)

    print()

def test_config_create(setup: BuildSetup, cases: List[CaseSetup]) -> None:
    print("# Creating configuration files")

    for case in cases:
        if case.failed:
            test_failed(case)
            continue

        cmd = [
            sys.executable,
            str(setup.helpers.generator),
            "-x", str(case.slice),
            "-c", str(setup.config.consts)
        ]

        try:
            with case.config.open("w", encoding="utf-8") as fout:
                proc = subprocess.run(
                    cmd,
                    text=True,
                    stdout=fout,
                    stderr=subprocess.PIPE
                )
            if proc.returncode != 0:
                test_failed(case)
                print_test_output(proc.stderr)
            else:
                test_passed(case)

        except Exception as e:
            test_failed(case)
    print()

def test_source_compile(setup: BuildSetup, cases: List[CaseSetup]) -> None:
    print("# Compilation")

    for case in cases:
        if case.failed:
            test_failed(case)
            continue

        cmd = [
            str(setup.compiler.ca65),
            "-I", str(case.root),
            "-o", str(case.object),
            setup.config.source
        ]

        try:
            proc = subprocess.run(
                cmd,
                text=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT
            )
            if proc.returncode != 0:
                test_failed(case)
                print_test_output(proc.stdout)
            else:
                test_passed(case)

        except:
            test_failed(case)
    print()

def test_object_link(setup: BuildSetup, cases: List[CaseSetup]) -> None:
    print("# Linking")

    for case in cases:
        if case.failed:
            test_failed(case)
            continue

        cmd = [
            str(setup.compiler.ld65),
            "-C", str(setup.config.memory),
            "-o", str(case.target) ,
            str(case.object)
        ]

        try:
            proc = subprocess.run(
                cmd,
                text=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT
            )
            if proc.returncode != 0:
                test_failed(case)
                print_test_output(proc.stdout)
            else:
                test_passed(case)

        except:
            test_failed(case)
    print()

def test_reference_show(setup: BuildSetup, case : CaseSetup) -> None:
    print("    Expected:")
    print_rom_info(setup, case.reference)

    print("    Received:")
    print_rom_info(setup, case.target)

def test_reference_compare(setup: BuildSetup, cases: List[CaseSetup]) -> None:
    print("# Comparing the result")

    for case in cases:
        if case.failed:
            test_failed(case)
            continue

        if filecmp.cmp(case.target, case.reference, shallow=False):
            test_passed(case)
        else:
            test_reference_show(setup, case)
            test_failed(case)
    print()

def test_final(cases: List[CaseSetup]) -> None:
    print("# Total results")

    total  = 0
    passed = 0

    for case in cases:
        if case.failed:
            test_failed(case)
        else:
            test_passed(case)
            passed += 1

        total += 1

    print()
    print(f" - Passed: {passed} out of {total}")

def build_printer(setup: BuildSetup) -> None:
    print("# Building prerequisites")

    cmd = [
        str(setup.compiler.make),
        "-C", str(setup.directory.reader)
    ]
    cmd_str = " ".join(cmd)

    try:
        proc = subprocess.run(
            cmd,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )
        if proc.returncode != 0:
            print_failed(f"{cmd_str}")
            print_test_output(proc.stderr)
        else:
            print_passed(f"{cmd_str}")

    except Exception as e:
        print_failed(f"{cmd_str}: {e}")
    print()


def main() -> None:
    TEST_STEPS = [
        test_directory_clean,
        test_config_create,
        test_source_compile,
        test_object_link,
        test_reference_compare,
        test_directory_clean
    ]

    setup = build_setup()

    build_printer(setup)
    cases = cases_setup(setup)

    for step in TEST_STEPS:
        step(setup, cases)

    test_final(cases)

if __name__ == "__main__":
    main()
