from __future__ import annotations

import subprocess
import tempfile

from pathlib import Path
from typing import List

from tests.helpers.output import print_failed, print_passed
from tests.helpers.png_compose import compose_chr_template
from tests.helpers.smoke import SmokeCase, run_smoke_cases

from subroutines.common import (
    check_success,
    fixture,
    print_process_output,
    required_file,
    template_2c02,
)


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


def _run(
    program: Path,
    args: List[str]
) -> subprocess.CompletedProcess[str]:
    """Run chr-convert."""

    return subprocess.run(
        [str(program), *args],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )


def _check_conversion(
    name: str,
    proc: subprocess.CompletedProcess[str]
) -> bool:
    """Check one conversion smoke result."""

    error = check_success(name, proc)
    if error:
        print_failed(name, error)
        print_process_output(proc)
        return False

    print_passed(name)
    return True


def run_conversion_smoke(program: Path, root: Path) -> bool:
    """Run conversion smoke tests."""

    passed = 0
    total = 3

    print("# Conversion")

    with tempfile.TemporaryDirectory() as tmp:
        tmpdir = Path(tmp)
        full = tmpdir / "components-full.png"
        compose_chr_template(
            full,
            fixture(root, "colors.png"),
            fixture(root, "palettes0.png"),
            fixture(root, "table0.png"),
            fixture(root, "table1.png")
        )

        proc = _run(
            program,
            [
                "-q",
                "-i",
                str(template_2c02(root)),
                "-C",
                str(tmpdir / "colors.pal"),
                "-P",
                str(tmpdir / "palettes.spals"),
                "-T",
                str(tmpdir / "tables.pats")
            ]
        )
        if _check_conversion("full template to binaries", proc):
            passed += 1

        proc = _run(
            program,
            [
                "-q",
                "-c",
                str(fixture(root, "colors.png")),
                "-p",
                str(fixture(root, "palettes0.png")),
                "-l",
                str(fixture(root, "table0.png")),
                "-r",
                str(fixture(root, "table1.png")),
                "-T",
                str(tmpdir / "component-tables.pats")
            ]
        )
        if _check_conversion("components to binaries", proc):
            passed += 1

        proc = _run(
            program,
            [
                "-q",
                "-i",
                str(full),
                "-T",
                str(tmpdir / "composed-tables.pats")
            ]
        )
        if _check_conversion("composed full template to binaries", proc):
            passed += 1

    print()
    print(f" - Passed: {passed} out of {total}")
    return passed == total


def run_smoke_tests(root: Path, program: Path) -> int:
    """Run the chr-convert smoke tests."""

    binary = required_file(program)
    basic_status = run_smoke_cases(binary, basic_cases())
    print()

    conversion_ok = run_conversion_smoke(binary, root)
    return 0 if basic_status == 0 and conversion_ok else 1
