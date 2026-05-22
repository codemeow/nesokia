from __future__ import annotations

import subprocess
import tempfile

from pathlib import Path

from tests.helpers.output import print_failed, print_indented, print_passed

from subroutines.common import (
    first_diff,
    golden_root,
    print_process_output,
    required_file,
    template_2c02,
)


OUTPUTS = [
    ("colors", "-C", "2c02-template.pal", "actual.pal"),
    ("palettes", "-P", "2c02-template.spals", "actual.spals"),
    ("pattern tables", "-T", "2c02-template.pats", "actual.pats")
]


def run_golden_tests(root: Path, program: Path) -> int:
    """Run the chr-convert golden file tests."""

    binary = required_file(program)
    source = template_2c02(root)
    passed = 0
    total = len(OUTPUTS)

    print("# Golden")

    with tempfile.TemporaryDirectory() as tmp:
        tmpdir = Path(tmp)

        for name, option, expected_name, actual_name in OUTPUTS:
            expected = required_file(golden_root(root) / expected_name)
            actual = tmpdir / actual_name
            proc = subprocess.run(
                [
                    str(binary),
                    "-q",
                    "-i",
                    str(source),
                    option,
                    str(actual)
                ],
                text=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )

            if proc.returncode != 0:
                print_failed(name, f"exit code {proc.returncode}")
                print_process_output(proc)
                continue

            if proc.stdout.strip() or proc.stderr.strip():
                print_failed(name, "unexpected output")
                print_process_output(proc)
                continue

            actual_bytes = actual.read_bytes()
            expected_bytes = expected.read_bytes()
            if actual_bytes != expected_bytes:
                print_failed(name, "file differs from golden")
                print_indented(first_diff(expected_bytes, actual_bytes))
                continue

            print_passed(name)
            passed += 1

    print()
    print(f" - Passed: {passed} out of {total}")
    return 0 if passed == total else 1
