#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from __future__ import annotations

import filecmp
import shutil
import subprocess
import sys

from dataclasses import dataclass
from pathlib import Path
from typing import IO, Optional, Union, List

from tests.helpers.nes_cases import nes_cases
from tests.helpers.output import print_failed, print_indented, print_passed
from tests.helpers.runner import run

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent.parent.parent
SubprocessOutput = Union[int, IO[str]]


@dataclass
class BuildSetup:
    """Local setup values."""

    root: Path

    @dataclass
    class BuildCompiler:
        """Compiler programs."""

        ca65: Path
        ld65: Path

    @dataclass
    class BuildHelpers:
        """Helper utilities."""

        generator: Path
        reader: Path

    @dataclass
    class BuildConfig:
        """Configuration files for the tests."""

        consts: Path
        source: Path
        memory: Path

    compiler: BuildCompiler
    helpers: BuildHelpers
    config: BuildConfig


@dataclass
class CaseSetup:
    """One test case setup."""

    root: Path
    meta: Path
    object_file: Path
    target: Path
    config: Path
    reference: Path
    failed: bool


def required_file(path: Union[str, Path]) -> Path:
    """Resolve a file path and require it to exist."""

    filename = Path(path).resolve()
    if not filename.is_file():
        raise RuntimeError(f"File {filename} not found")
    return filename


def output_file(path: Union[str, Path]) -> Path:
    """Resolve a generated file path."""

    return Path(path).resolve()


def resolve_program(name: str) -> Path:
    """Resolve a program path from PATH."""

    prog = shutil.which(name)
    if prog is None:
        raise RuntimeError(f"{name} not found in PATH")
    return Path(prog)


def build_setup() -> BuildSetup:
    """Build the global test setup."""

    return BuildSetup(
        root=ROOT,
        compiler=BuildSetup.BuildCompiler(
            ca65=resolve_program("ca65"),
            ld65=resolve_program("ld65")
        ),
        helpers=BuildSetup.BuildHelpers(
            generator=required_file(HERE / "subroutines/nsk_config_create.py"),
            reader=required_file(ROOT / "bin/nesokia-ines-inspect")
        ),
        config=BuildSetup.BuildConfig(
            consts=required_file(
                ROOT / "utils/ines/header/nsk_header_consts.inc"
            ),
            source=required_file(
                ROOT / "utils/ines/header/nsk_header_code.asm"
            ),
            memory=required_file(HERE / "memory/nsk_header_memory.cfg")
        )
    )


def print_case_passed(case: CaseSetup) -> None:
    """Print a test case as passed."""

    print_passed(case.root.name)


def print_case_failed(case: CaseSetup, e: Optional[Exception] = None) -> None:
    """Print a test case as failed."""

    if e is None:
        print_failed(case.root.name)
    else:
        print_failed(case.root.name, str(e))


def mark_case_failed(case: CaseSetup, e: Optional[Exception] = None) -> None:
    """Mark a test case as failed and print it."""

    print_case_failed(case, e)
    case.failed = True


def run_case_command(
    case: CaseSetup,
    cmd: List[str],
    stdout: SubprocessOutput = subprocess.PIPE,
    stderr: SubprocessOutput = subprocess.STDOUT
) -> Optional[subprocess.CompletedProcess[str]]:
    """Run a test case command and mark the case failed on exceptions."""

    try:
        return subprocess.run(
            cmd,
            text=True,
            stdout=stdout,
            stderr=stderr
        )
    except Exception as e:
        mark_case_failed(case, e)
        return None


def print_rom_info(setup: BuildSetup, rom_file: Path) -> None:
    """Print selected ROM info through the reader utility."""

    cmd = [
        str(setup.helpers.reader),
        str(rom_file)
    ]
    try:
        proc = run(
            cmd,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT
        )
    except Exception as e:
        print_indented(f"Unable to inspect {rom_file}: {e}")
        return

    print_indented(proc.stdout)
    if proc.returncode != 0:
        print_indented(f"Reader exited with code {proc.returncode}")


def cases_setup(setup: BuildSetup) -> List[CaseSetup]:
    """Populate the test cases list."""

    cases: List[CaseSetup] = []
    for case in nes_cases(setup.root):
        cases.append(
            CaseSetup(
                root=case.root,
                meta=case.meta,
                object_file=output_file(case.root / "test.o"),
                target=output_file(case.root / "test.nes"),
                config=output_file(case.root / "nsk_header_config.inc"),
                reference=case.reference,
                failed=False))
    return cases


def test_directory_clean(setup: BuildSetup, cases: List[CaseSetup]) -> None:
    """Remove generated files from the test cases."""

    print("# Clearing old files")

    for case in cases:
        case.object_file.unlink(missing_ok=True)
        case.target.unlink(missing_ok=True)
        case.config.unlink(missing_ok=True)

        if case.failed:
            print_case_failed(case)
        else:
            print_case_passed(case)

    print()


def test_config_create(setup: BuildSetup, cases: List[CaseSetup]) -> None:
    """Create configuration files for the test cases."""

    print("# Creating configuration files")

    for case in cases:
        if case.failed:
            print_case_failed(case)
            continue

        cmd = [
            sys.executable,
            str(setup.helpers.generator),
            "-x", str(case.meta),
            "-c", str(setup.config.consts)
        ]

        try:
            with case.config.open("w", encoding="utf-8") as fout:
                proc = run_case_command(
                    case,
                    cmd,
                    stdout=fout,
                    stderr=subprocess.PIPE
                )
        except Exception as e:
            mark_case_failed(case, e)
            continue

        if proc is None:
            continue

        if proc.returncode != 0:
            mark_case_failed(case)
            print_indented(proc.stderr or "")
        else:
            print_case_passed(case)
    print()


def test_source_compile(setup: BuildSetup, cases: List[CaseSetup]) -> None:
    """Compile the test cases."""

    print("# Compilation")

    for case in cases:
        if case.failed:
            print_case_failed(case)
            continue

        cmd = [
            str(setup.compiler.ca65),
            "-I", str(case.root),
            "-o", str(case.object_file),
            str(setup.config.source)
        ]

        proc = run_case_command(case, cmd)
        if proc is None:
            continue

        if proc.returncode != 0:
            mark_case_failed(case)
            print_indented(proc.stdout or "")
        else:
            print_case_passed(case)
    print()


def test_object_link(setup: BuildSetup, cases: List[CaseSetup]) -> None:
    """Link the test cases."""

    print("# Linking")

    for case in cases:
        if case.failed:
            print_case_failed(case)
            continue

        cmd = [
            str(setup.compiler.ld65),
            "-C", str(setup.config.memory),
            "-o", str(case.target),
            str(case.object_file)
        ]

        proc = run_case_command(case, cmd)
        if proc is None:
            continue

        if proc.returncode != 0:
            mark_case_failed(case)
            print_indented(proc.stdout or "")
        else:
            print_case_passed(case)
    print()


def test_reference_show(setup: BuildSetup, case: CaseSetup) -> None:
    """Show the linked and reference ROMs data."""

    print("    Expected:")
    print_rom_info(setup, case.reference)

    print("    Received:")
    print_rom_info(setup, case.target)


def test_reference_compare(setup: BuildSetup, cases: List[CaseSetup]) -> None:
    """Compare the generated and reference ROMs."""

    print("# Comparing the result")

    for case in cases:
        if case.failed:
            print_case_failed(case)
            continue

        if filecmp.cmp(case.target, case.reference, shallow=False):
            print_case_passed(case)
        else:
            test_reference_show(setup, case)
            mark_case_failed(case)
    print()


def test_final(cases: List[CaseSetup]) -> int:
    """Show the final test results and return the process status."""

    print("# Final results")

    total = 0
    passed = 0

    for case in cases:
        if case.failed:
            print_case_failed(case)
        else:
            print_case_passed(case)
            passed += 1

        total += 1

    print()
    print(f" - Passed: {passed} out of {total}")

    return 0 if passed == total else 1


def main() -> int:
    """Run the test suite."""

    test_steps = [
        test_directory_clean,
        test_config_create,
        test_source_compile,
        test_object_link,
        test_reference_compare,
        test_directory_clean
    ]

    setup = build_setup()

    cases = cases_setup(setup)

    for step in test_steps:
        step(setup, cases)

    return test_final(cases)


if __name__ == "__main__":
    rc = main()
    sys.exit(rc)
