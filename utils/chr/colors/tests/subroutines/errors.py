from __future__ import annotations

import subprocess
import tempfile

from dataclasses import dataclass, field
from pathlib import Path
from typing import List

from tests.helpers.output import print_failed, print_indented, print_passed
from tests.helpers.runner import run

from subroutines.common import required_file, template_2c02


@dataclass(frozen=True)
class ErrorCase:
    """One error handling command setup."""

    name: str
    args: List[str]
    output: Path
    stderr_contains: List[str] = field(default_factory=list)


def print_process_output(proc: subprocess.CompletedProcess[str]) -> None:
    """Print captured process output for a failed error handling test."""

    if proc.stdout:
        print_indented("stdout:")
        print_indented(proc.stdout.rstrip())
    if proc.stderr:
        print_indented("stderr:")
        print_indented(proc.stderr.rstrip())


def error_cases(root: Path, tmpdir: Path) -> List[ErrorCase]:
    """Build error handling test cases."""

    input_file = template_2c02(root)
    unknown_input = tmpdir / "2c02.unknown"
    bad_pal = tmpdir / "bad.pal"
    bad_png = tmpdir / "bad.png"

    unknown_input.write_bytes(input_file.read_bytes())
    bad_pal.write_bytes(b"\x00")
    bad_png.write_text("not a png\n", encoding="utf-8")

    return [
        ErrorCase(
            name="missing input file",
            args=[
                "-q",
                "-i",
                str(tmpdir / "missing.pal"),
                "-o",
                str(tmpdir / "missing-input.gpl")
            ],
            output=tmpdir / "missing-input.gpl",
            stderr_contains=["cannot open file"]
        ),
        ErrorCase(
            name="unknown input extension",
            args=[
                "-q",
                "-i",
                str(unknown_input),
                "-o",
                str(tmpdir / "unknown-input.gpl")
            ],
            output=tmpdir / "unknown-input.gpl",
            stderr_contains=["Unknown input file extension"]
        ),
        ErrorCase(
            name="unknown output extension",
            args=[
                "-q",
                "-i",
                str(input_file),
                "-o",
                str(tmpdir / "unknown-output.unknown")
            ],
            output=tmpdir / "unknown-output.unknown",
            stderr_contains=["Unknown output file extension"]
        ),
        ErrorCase(
            name="invalid PAL size",
            args=[
                "-q",
                "-i",
                str(bad_pal),
                "-o",
                str(tmpdir / "bad-pal.gpl")
            ],
            output=tmpdir / "bad-pal.gpl",
            stderr_contains=["cannot read"]
        ),
        ErrorCase(
            name="invalid PNG input",
            args=[
                "-q",
                "-i",
                str(bad_png),
                "-I",
                "png",
                "-o",
                str(tmpdir / "bad-png.pal")
            ],
            output=tmpdir / "bad-png.pal",
            stderr_contains=["PNG"]
        ),
        ErrorCase(
            name="unwritable output path",
            args=[
                "-q",
                "-i",
                str(input_file),
                "-o",
                str(tmpdir / "missing-dir" / "out.gpl")
            ],
            output=tmpdir / "missing-dir" / "out.gpl",
            stderr_contains=["cannot open file"]
        )
    ]


def run_error_case(program: Path, case: ErrorCase) -> bool:
    """Run one error handling test case."""

    proc = run(
        [str(program), *case.args],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )

    if proc.returncode == 0:
        print_failed(case.name, "exit code 0")
        print_process_output(proc)
        return False

    if proc.stdout.strip():
        print_failed(case.name, "stdout is not empty")
        print_process_output(proc)
        return False

    if not proc.stderr.strip():
        print_failed(case.name, "stderr is empty")
        print_process_output(proc)
        return False

    for value in case.stderr_contains:
        if value not in proc.stderr:
            print_failed(case.name, f"stderr does not contain {value!r}")
            print_process_output(proc)
            return False

    if case.output.exists():
        print_failed(case.name, f"output exists: {case.output}")
        return False

    print_passed(case.name)
    return True


def run_error_tests(root: Path, program: Path) -> int:
    """Run the chr-colors error handling tests."""

    binary = required_file(program)
    passed = 0

    print("# Error Handling")

    with tempfile.TemporaryDirectory() as tmp:
        cases = error_cases(root, Path(tmp))

        for case in cases:
            if run_error_case(binary, case):
                passed += 1

    total = len(cases)
    print()
    print(f" - Passed: {passed} out of {total}")
    return 0 if passed == total else 1
