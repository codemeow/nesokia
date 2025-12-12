#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import filecmp
import shutil
import subprocess
import sys
import textwrap

from colorama import init, Fore, Style
from dataclasses import dataclass
from pathlib import Path
from typing import Union, List

# Colorama's init
init(autoreset=True)

##
## \brief  Local setup values
##
@dataclass
class BuildSetup:
    ##
    ## \brief  Compiler programs
    ##
    @dataclass
    class BuildCompiler:
        ca65: Path ##< Path to the ca65 compiler
        ld65: Path ##< Path to the ld65 linker
        make: Path ##< Path to the make program

    ##
    ## \brief  List of helpful directories
    ##
    @dataclass
    class BuildDirectories:
        cases  : Path ##< Path to the test cases
        helpers: Path ##< Path to the test subroutines
        reader : Path ##< Path to the ROM reader utility

    ##
    ## \brief  List of helpers utilities
    ##
    @dataclass
    class BuildHelpers:
        generator: Path ##< Path to the ROM header generator
        reader   : Path ##< Path to the ROM header reader

    ##
    ## \brief  Configuration files for the tests
    ##
    @dataclass
    class BuildConfig:
        consts: Path ##< Generator's constants file
        source: Path ##< Generator's source file
        memory: Path ##< Generator's linker script

    compiler:  BuildCompiler        ##< Compiler programs
    directory: BuildDirectories     ##< List of helpful directories
    helpers:   BuildHelpers         ##< List of helper utilities
    config:    BuildConfig          ##< Configuration files for the tests

##
## \brief  One test case setup
##
@dataclass
class CaseSetup:
    root      : Path ##< Path to the root directory of the test case
    slice     : Path ##< Path to the NES20DB slice for that test case
    object    : Path ##< Path to the compiled object file
    target    : Path ##< Path to the linked target test file
    config    : Path ##< Path to the generator's configuration file
    reference : Path ##< Path to the reference ROM header file
    failed    : bool ##< Failed test flag

##
## \brief  Resolves the filename and checks its existence
##
## \param  path         The path (string or Path)
## \param  missing_ok   If True, missing file will not raise an error
## \return filename as Path type
##
def resolve_file(path: Union[str, Path], missing_ok: bool = False) -> Path:
    file = Path(path).resolve()
    if not file.is_file() and not missing_ok:
        raise RuntimeError(f"File {file} not found")
    return file

##
## \brief  Resolves the directory name and checks its existence
##
## \param  path         The path (string or Path)
## \param  missing_ok   If True, missing directory will not raise an error
## \return Directory name as Path type
##
def resolve_dir(path: Union[str, Path], missing_ok: bool = False) -> Path:
    directory = Path(path).resolve()
    if not directory.is_dir() and not missing_ok:
        raise RuntimeError(f"Directory {directory} not found")
    return directory

##
## \brief  Resolves the program name
##
## \param  name  The program name
## \return Extracted from PATH full name
##
def resolve_program(name : str) -> Path:
    prog = shutil.which(name)
    if prog is None:
        raise RuntimeError(f"{name} not found in PATH")
    return Path(prog)

##
## \brief  Builds a global setup.
##
## \return The setup.
##
def build_setup() -> BuildSetup:
    here = Path(__file__).resolve().parent
    root = here.parent.parent.parent

    return BuildSetup(
        compiler = BuildSetup.BuildCompiler(
            ca65 = resolve_program("ca65"),
            ld65 = resolve_program("ld65"),
            make = resolve_program("make")
        ),
        directory = BuildSetup.BuildDirectories(
            cases   = resolve_dir(here / "cases"),
            helpers = resolve_dir(here / "subroutines"),
            reader  = resolve_dir(root / "utils/head")
        ),
        helpers = BuildSetup.BuildHelpers(
            generator = resolve_file(here / "subroutines/nsk_config_create.py"),
            reader    = resolve_file(root / "bin/nesokia-head")
        ),
        config = BuildSetup.BuildConfig(
            consts = resolve_file(root / "utils/create/nsk_header_consts.inc"),
            source = resolve_file(root / "utils/create/nsk_header_code.asm"),
            memory = resolve_file(here / "memory/nsk_header_memory.cfg")
        )
    )

##
## \brief  Prints the text as "passed" line
##
## \param  string  The string
##
def print_passed(string: str) -> None:
    print(f"[{Fore.GREEN}OK{Style.RESET_ALL}]    {string}")

##
## \brief  Prints the text as "failed" line
##
## \param  string  The string
##
def print_failed(string: str) -> None:
    print(f"[{Fore.RED}ERR{Style.RESET_ALL}]   {string}")

##
## \brief  Prints the case as "passed"
##
## \param  case  The test case
##
def test_passed(case: CaseSetup) -> None:
    print_passed(case.root.name)

##
## \brief  Prints the case as "failed"
##
## \param  case  The test case
##
def test_failed(case: CaseSetup, e: Exception = None) -> None:
    if e is None:
        print_failed(case.root.name)
    else:
        print_failed(f"{case.root.name} : {e}")
    case.failed = True

##
## \brief  Prints the additional text with the indentation
##
## \param  output  The output
##
def print_test_output(output: str) -> None:
    print(f"{textwrap.indent(output, '    ')}")

##
## \brief  Prints the selected ROM info
##
## \param  setup  The setup
## \param  file   The ROM filename
##
def print_rom_info(setup: BuildSetup, file: Path) -> None:
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

##
## \brief  Populates the test cases list
##
## \param  setup  The setup
## \return test cases list
##
def cases_setup(setup: BuildSetup) -> List[CaseSetup]:
    cases: List[CaseSetup] = []
    for sub in sorted(setup.directory.cases.iterdir()):
        if not sub.is_dir():
            continue

        cases.append(
            CaseSetup(
                root        = sub,
                slice       = resolve_file(sub / "nes20db-slice.xml"),
                object      = resolve_file(sub / "test.o",                  missing_ok = True),
                target      = resolve_file(sub / "test.nes",                missing_ok = True),
                config      = resolve_file(sub / "nsk_header_config.inc",   missing_ok = True),
                reference   = resolve_file(sub / "rom-header.nes"),
                failed      = False
            )
        )
    return cases

##
## \brief  Cleans the test cases directory of the generated files
##
## \param  setup  The setup
## \param  cases  The test cases
##
def test_directory_clean(setup: BuildSetup, cases: List[CaseSetup]) -> None:
    print("# Clearing old files")

    for case in cases:
        if case.failed:
            test_failed(case)
            continue

        case.object.unlink(missing_ok = True)
        case.target.unlink(missing_ok = True)
        case.config.unlink(missing_ok = True)
        test_passed(case)

    print()

##
## \brief  Creates the configuration files for the test cases
##
## \param  setup  The setup
## \param  cases  The test cases
##
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
            test_failed(case, e)
    print()

##
## \brief  Compiles the test cases
##
## \param  setup  The setup
## \param  cases  The test cases
##
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

        except Exception as e:
            test_failed(case, e)
    print()

##
## \brief  Links the test cases
##
## \param  setup  The setup
## \param  cases  The test cases
##
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

        except Exception as e:
            test_failed(case, e)
    print()


##
## \brief  Shows the linked and reference ROMs data
##
## \param  setup  The setup
## \param  cases  The test cases
##
def test_reference_show(setup: BuildSetup, case : CaseSetup) -> None:
    print("    Expected:")
    print_rom_info(setup, case.reference)

    print("    Received:")
    print_rom_info(setup, case.target)

##
## \brief  Compares the generated and reference ROMs
##
## \param  setup  The setup
## \param  cases  The test cases
##
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

##
## \brief  Shows the final test results
##
## \param  setup  The setup
## \param  cases  The test cases
##
## \return 0 if all the tests are successfull
##
def test_final(cases: List[CaseSetup]) -> int:
    print("# Final results")

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

    return 0 if passed == total else 1


##
## \brief  Builds the ROM reader program
##
## \param  setup  The setup
##
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
            sys.exit(1)
        else:
            print_passed(f"{cmd_str}")

    except Exception as e:
        print_failed(f"{cmd_str}: {e}")
        sys.exit(1)

    print()

##
## \brief  Main function
##
def main() -> int:
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

    return test_final(cases)

if __name__ == "__main__":
    rc = main()
    sys.exit(rc)
