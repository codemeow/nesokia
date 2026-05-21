from __future__ import annotations

import subprocess
import tempfile

from pathlib import Path

from tests.helpers.output import print_failed, print_indented, print_passed

from subroutines.common import golden_root, required_file, template_2c02


FORMATS = [
    "gpl",
    "aco",
    "ase",
    "png"
]


def print_process_output(proc: subprocess.CompletedProcess[str]) -> None:
    """Print captured process output for a failed golden test."""

    if proc.stdout:
        print_indented("stdout:")
        print_indented(proc.stdout.rstrip())
    if proc.stderr:
        print_indented("stderr:")
        print_indented(proc.stderr.rstrip())


def first_diff(expected: bytes, actual: bytes) -> str:
    """Return a compact byte-level diff summary."""

    for i, expected_byte in enumerate(expected):
        if i >= len(actual):
            return f"actual ended at byte {i}"
        if actual[i] != expected_byte:
            return (
                f"byte {i}: got 0x{actual[i]:02x}, "
                f"expected 0x{expected_byte:02x}"
            )

    if len(actual) != len(expected):
        return f"actual has {len(actual) - len(expected)} extra bytes"

    return "unknown byte difference"


def run_golden_tests(root: Path, program: Path) -> int:
    """Run the chr-colors golden file tests."""

    binary = required_file(program)
    input_file = template_2c02(root)
    passed = 0
    total = len(FORMATS)

    print("# Golden")

    with tempfile.TemporaryDirectory() as tmp:
        tmpdir = Path(tmp)

        for output_format in FORMATS:
            expected = required_file(
                golden_root(root) / f"2c02.{output_format}"
            )
            actual = tmpdir / f"2c02.{output_format}"
            proc = subprocess.run(
                [
                    str(binary),
                    "-q",
                    "-i",
                    str(input_file),
                    "-o",
                    str(actual)
                ],
                text=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )

            if proc.returncode != 0:
                print_failed(
                    output_format,
                    f"exit code {proc.returncode}"
                )
                print_process_output(proc)
                continue

            if proc.stdout.strip() or proc.stderr.strip():
                print_failed(output_format, "unexpected output")
                print_process_output(proc)
                continue

            if actual.read_bytes() != expected.read_bytes():
                print_failed(output_format, "file differs from golden")
                print_indented(
                    first_diff(expected.read_bytes(), actual.read_bytes())
                )
                continue

            print_passed(output_format)
            passed += 1

    print()
    print(f" - Passed: {passed} out of {total}")
    return 0 if passed == total else 1
