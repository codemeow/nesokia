from __future__ import annotations

import subprocess
import tempfile

from pathlib import Path
from typing import List

from tests.helpers.output import print_failed, print_indented, print_passed
from tests.helpers.runner import run
from tests.helpers.smoke import SmokeCase, run_smoke_cases

from subroutines.common import required_file, template_2c02


OUTPUT_FORMATS = [
    "pal",
    "gpl",
    "aco",
    "ase",
    "png"
]


def basic_cases() -> List[SmokeCase]:
    """Build basic smoke test cases."""

    return [
        SmokeCase(
            section="Basic",
            name="help short",
            args=["-h"],
            stdout_required=True,
            stderr_forbidden=True
        ),
        SmokeCase(
            section="Basic",
            name="help long",
            args=["--help"],
            stdout_required=True,
            stderr_forbidden=True
        ),
        SmokeCase(
            section="Basic",
            name="version short",
            args=["-v"],
            stdout_required=True,
            stderr_forbidden=True
        ),
        SmokeCase(
            section="Basic",
            name="version long",
            args=["--version"],
            stdout_required=True,
            stderr_forbidden=True
        )
    ]


def print_process_output(proc: subprocess.CompletedProcess[str]) -> None:
    """Print captured process output for a failed smoke test."""

    if proc.stdout:
        print_indented("stdout:")
        print_indented(proc.stdout.rstrip())
    if proc.stderr:
        print_indented("stderr:")
        print_indented(proc.stderr.rstrip())


def run_conversion_smoke(program: Path, root: Path) -> bool:
    """Run file conversion smoke tests."""

    passed = 0
    total = len(OUTPUT_FORMATS) + 1
    input_file = template_2c02(root)

    print("# Conversion")

    with tempfile.TemporaryDirectory() as tmp:
        tmpdir = Path(tmp)
        png_file = tmpdir / "2c02.png"

        for output_format in OUTPUT_FORMATS:
            output_file = tmpdir / f"2c02.{output_format}"
            proc = run(
                [
                    str(program),
                    "-q",
                    "-i",
                    str(input_file),
                    "-o",
                    str(output_file)
                ],
                text=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )

            if proc.returncode != 0:
                print_failed(
                    f"pal to {output_format}",
                    f"exit code {proc.returncode}"
                )
                print_process_output(proc)
                continue

            if proc.stdout.strip() or proc.stderr.strip():
                print_failed(f"pal to {output_format}", "unexpected output")
                print_process_output(proc)
                continue

            if not output_file.is_file() or output_file.stat().st_size == 0:
                print_failed(f"pal to {output_format}", "output is missing")
                continue

            print_passed(f"pal to {output_format}")
            passed += 1

        proc = run(
            [
                str(program),
                "-q",
                "-i",
                str(png_file),
                "-o",
                str(tmpdir / "from-png.pal")
            ],
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )

        if proc.returncode != 0:
            print_failed("png to pal", f"exit code {proc.returncode}")
            print_process_output(proc)
        elif proc.stdout.strip() or proc.stderr.strip():
            print_failed("png to pal", "unexpected output")
            print_process_output(proc)
        else:
            print_passed("png to pal")
            passed += 1

    print()
    print(f" - Passed: {passed} out of {total}")
    return passed == total


def run_smoke_tests(root: Path, program: Path) -> int:
    """Run the chr-colors smoke tests."""

    binary = required_file(program)
    basic_status = run_smoke_cases(binary, basic_cases())
    print()

    conversion_ok = run_conversion_smoke(binary, root)
    return 0 if basic_status == 0 and conversion_ok else 1
