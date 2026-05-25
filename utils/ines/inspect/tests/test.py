#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from pathlib import Path

from subroutines.cli import run_cli_tests
from subroutines.directories import run_directory_tests
from subroutines.errors import run_error_tests
from subroutines.golden import run_golden_tests
from subroutines.semantic import run_semantic_tests
from subroutines.smoke import run_smoke_tests

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent.parent.parent
PROGRAM = ROOT / "bin/nesokia-ines-inspect"


def main() -> int:
    """Run the inspect tests."""

    smoke_status = run_smoke_tests(ROOT, PROGRAM)
    print()
    cli_status = run_cli_tests(ROOT, PROGRAM)
    print()
    golden_status = run_golden_tests(ROOT, PROGRAM)
    print()
    semantic_status = run_semantic_tests(ROOT, PROGRAM)
    print()
    error_status = run_error_tests(ROOT, PROGRAM)
    print()
    directory_status = run_directory_tests(ROOT, PROGRAM)

    if (
        smoke_status == 0
        and cli_status == 0
        and golden_status == 0
        and semantic_status == 0
        and error_status == 0
        and directory_status == 0
    ):
        return 0

    return 1


if __name__ == "__main__":
    raise SystemExit(main())
