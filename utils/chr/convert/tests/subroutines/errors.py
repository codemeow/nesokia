from __future__ import annotations

import tempfile

from dataclasses import dataclass, field
from pathlib import Path
from typing import List

from tests.helpers.output import print_failed, print_passed

from subroutines.common import (
    fixture,
    print_process_output,
    required_file,
    run_command,
    template_2c02,
)


@dataclass(frozen=True)
class ErrorCase:
    """One error handling command setup."""

    name: str
    args: List[str]
    output: Path
    stderr_contains: List[str] = field(default_factory=list)


def error_cases(root: Path, tmpdir: Path) -> List[ErrorCase]:
    """Build error handling test cases."""

    bad_png = tmpdir / "bad.png"
    bad_pal = tmpdir / "bad.pal"
    bad_spals = tmpdir / "bad.spals"
    bad_pats = tmpdir / "bad.pats"

    bad_png.write_text("not a png\n", encoding="utf-8")
    bad_pal.write_bytes(b"\x00")
    bad_spals.write_bytes(b"\x00")
    bad_pats.write_bytes(b"\x00")

    full = template_2c02(root)
    colors = fixture(root, "colors.png")
    palettes = fixture(root, "palettes0.png")
    table0 = fixture(root, "table0.png")
    table1 = fixture(root, "table1.png")

    return [
        ErrorCase(
            name="missing t2c input",
            args=["-q", "-T", str(tmpdir / "missing-input.pats")],
            output=tmpdir / "missing-input.pats",
            stderr_contains=["no PPU colors PNG template"]
        ),
        ErrorCase(
            name="missing input file",
            args=[
                "-q",
                "-i",
                str(tmpdir / "missing.png"),
                "-T",
                str(tmpdir / "missing-file.pats")
            ],
            output=tmpdir / "missing-file.pats",
            stderr_contains=["Cannot open file"]
        ),
        ErrorCase(
            name="full input with component input",
            args=[
                "-q",
                "-i",
                str(full),
                "-c",
                str(colors),
                "-T",
                str(tmpdir / "mixed.pats")
            ],
            output=tmpdir / "mixed.pats",
            stderr_contains=["no other input options"]
        ),
        ErrorCase(
            name="t2c full output forbidden",
            args=[
                "-q",
                "-i",
                str(full),
                "-O",
                str(tmpdir / "out.png")
            ],
            output=tmpdir / "out.png",
            stderr_contains=["full template cannot be used"]
        ),
        ErrorCase(
            name="c2t full input forbidden",
            args=[
                "-q",
                "-M",
                "c2t",
                "-i",
                str(full),
                "-c",
                str(tmpdir / "unused.pal"),
                "-O",
                str(tmpdir / "out.png")
            ],
            output=tmpdir / "out.png",
            stderr_contains=["full input cannot be used"]
        ),
        ErrorCase(
            name="c2t missing palettes",
            args=[
                "-q",
                "-M",
                "c2t",
                "-c",
                str(tmpdir / "colors.pal"),
                "-t",
                str(tmpdir / "tables.pats"),
                "-O",
                str(tmpdir / "out.png")
            ],
            output=tmpdir / "out.png",
            stderr_contains=["no palettes input"]
        ),
        ErrorCase(
            name="c2t missing pattern tables",
            args=[
                "-q",
                "-M",
                "c2t",
                "-c",
                str(tmpdir / "colors.pal"),
                "-p",
                str(tmpdir / "palettes.spals"),
                "-O",
                str(tmpdir / "out.png")
            ],
            output=tmpdir / "out.png",
            stderr_contains=["no pattern tables input"]
        ),
        ErrorCase(
            name="invalid PNG input",
            args=[
                "-q",
                "-i",
                str(bad_png),
                "-T",
                str(tmpdir / "bad-png.pats")
            ],
            output=tmpdir / "bad-png.pats",
            stderr_contains=["PNG"]
        ),
        ErrorCase(
            name="palette color outside colors",
            args=[
                "-q",
                "-c",
                str(colors),
                "-p",
                str(fixture(root, "palettesX.png")),
                "-l",
                str(table0),
                "-r",
                str(table1),
                "-T",
                str(tmpdir / "bad-palettes.pats")
            ],
            output=tmpdir / "bad-palettes.pats",
            stderr_contains=["invalid color in the palette"]
        ),
        ErrorCase(
            name="table color outside palettes",
            args=[
                "-q",
                "-c",
                str(colors),
                "-p",
                str(palettes),
                "-l",
                str(fixture(root, "tableX.png")),
                "-r",
                str(table1),
                "-T",
                str(tmpdir / "bad-table.pats")
            ],
            output=tmpdir / "bad-table.pats",
            stderr_contains=["Cannot find matching palette"]
        ),
        ErrorCase(
            name="invalid PAL size",
            args=[
                "-q",
                "-M",
                "c2t",
                "-c",
                str(bad_pal),
                "-p",
                str(bad_spals),
                "-t",
                str(bad_pats),
                "-O",
                str(tmpdir / "bad-pal.png")
            ],
            output=tmpdir / "bad-pal.png",
            stderr_contains=["cannot read"]
        ),
        ErrorCase(
            name="unwritable output path",
            args=[
                "-q",
                "-i",
                str(full),
                "-T",
                str(tmpdir / "missing-dir" / "out.pats")
            ],
            output=tmpdir / "missing-dir" / "out.pats",
            stderr_contains=["cannot open file"]
        )
    ]


def run_error_case(program: Path, case: ErrorCase) -> bool:
    """Run one error handling test case."""

    proc = run_command(program, case.args)

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
    """Run the chr-convert error handling tests."""

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
