#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from pathlib import Path

from subroutines.smoke import run_smoke_tests

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent.parent.parent
PROGRAM = ROOT / "bin/nesokia-ines-inspect"


def main() -> int:
    """Run the inspect tests."""

    return run_smoke_tests(ROOT, PROGRAM)


if __name__ == "__main__":
    raise SystemExit(main())
