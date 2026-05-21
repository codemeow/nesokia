from __future__ import annotations

import re
import shutil
import subprocess
import tempfile

from pathlib import Path
from typing import List

from tests.helpers.output import print_failed, print_indented, print_passed

from subroutines.common import required_file, template_2c02


RGB_LINE = re.compile(r"^\s*(\d{1,3})\s+(\d{1,3})\s+(\d{1,3})\s*$")


def print_process_output(proc: subprocess.CompletedProcess[str]) -> None:
    """Print captured process output for a failed semantic test."""

    if proc.stdout:
        print_indented("stdout:")
        print_indented(proc.stdout.rstrip())
    if proc.stderr:
        print_indented("stderr:")
        print_indented(proc.stderr.rstrip())


def pal_colors(filename: Path) -> List[tuple[int, int, int]]:
    """Read PAL file colors as RGB triples."""

    data = filename.read_bytes()
    return [
        (data[i], data[i + 1], data[i + 2])
        for i in range(0, len(data), 3)
    ]


def gpl_colors(filename: Path) -> List[tuple[int, int, int]]:
    """Read RGB rows from a GPL file."""

    colors: List[tuple[int, int, int]] = []
    for line in filename.read_text(encoding="utf-8").splitlines():
        match = RGB_LINE.match(line)
        if not match:
            continue
        colors.append(tuple(int(value) for value in match.groups()))

    return colors


def run_command(
    program: Path,
    args: List[str]
) -> subprocess.CompletedProcess[str]:
    """Run chr-colors and capture output."""

    return subprocess.run(
        [str(program), *args],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )


def check_success(
    name: str,
    proc: subprocess.CompletedProcess[str]
) -> bool:
    """Check common successful command constraints."""

    if proc.returncode != 0:
        print_failed(name, f"exit code {proc.returncode}")
        print_process_output(proc)
        return False

    if proc.stdout.strip() or proc.stderr.strip():
        print_failed(name, "unexpected output")
        print_process_output(proc)
        return False

    return True


def run_pal_roundtrip(program: Path, root: Path, tmpdir: Path) -> bool:
    """Check that PAL to PAL keeps bytes unchanged."""

    input_file = template_2c02(root)
    output_file = tmpdir / "2c02-out.pal"
    proc = run_command(
        program,
        ["-q", "-i", str(input_file), "-o", str(output_file)]
    )

    if not check_success("pal to pal", proc):
        return False

    if output_file.read_bytes() != input_file.read_bytes():
        print_failed("pal to pal", "output bytes differ")
        return False

    print_passed("pal to pal")
    return True


def run_png_roundtrip(program: Path, root: Path, tmpdir: Path) -> bool:
    """Check PAL to PNG to PAL roundtrip."""

    input_file = template_2c02(root)
    png_file = tmpdir / "2c02.png"
    output_file = tmpdir / "2c02-from-png.pal"

    proc = run_command(
        program,
        ["-q", "-i", str(input_file), "-o", str(png_file)]
    )
    if not check_success("pal to png", proc):
        return False

    proc = run_command(
        program,
        ["-q", "-i", str(png_file), "-o", str(output_file)]
    )
    if not check_success("png to pal", proc):
        return False

    if output_file.read_bytes() != input_file.read_bytes():
        print_failed("pal to png to pal", "output bytes differ")
        return False

    print_passed("pal to png to pal")
    return True


def run_gpl_structure(program: Path, root: Path, tmpdir: Path) -> bool:
    """Check GPL output structure and RGB rows."""

    input_file = template_2c02(root)
    output_file = tmpdir / "2c02.gpl"
    proc = run_command(
        program,
        ["-q", "-i", str(input_file), "-o", str(output_file)]
    )

    if not check_success("gpl structure", proc):
        return False

    text = output_file.read_text(encoding="utf-8")
    if not text.startswith("GIMP Palette\n"):
        print_failed("gpl structure", "missing GIMP Palette header")
        return False

    if "Columns: 16\n" not in text:
        print_failed("gpl structure", "missing Columns: 16")
        return False

    if gpl_colors(output_file) != pal_colors(input_file):
        print_failed("gpl structure", "RGB rows differ from PAL input")
        return False

    print_passed("gpl structure")
    return True


def run_format_overrides(program: Path, root: Path, tmpdir: Path) -> bool:
    """Check explicit input and output format overrides."""

    input_file = template_2c02(root)
    renamed_input = tmpdir / "2c02.input"
    output_file = tmpdir / "2c02.output"

    shutil.copyfile(input_file, renamed_input)

    proc = run_command(
        program,
        [
            "-q",
            "-i",
            str(renamed_input),
            "-I",
            "pal",
            "-o",
            str(output_file),
            "-O",
            "pal"
        ]
    )

    if not check_success("format overrides", proc):
        return False

    if output_file.read_bytes() != input_file.read_bytes():
        print_failed("format overrides", "output bytes differ")
        return False

    print_passed("format overrides")
    return True


def run_semantic_tests(root: Path, program: Path) -> int:
    """Run the chr-colors semantic tests."""

    binary = required_file(program)

    checks = [
        run_pal_roundtrip,
        run_png_roundtrip,
        run_gpl_structure,
        run_format_overrides
    ]

    passed = 0
    total = len(checks)
    print("# Semantic")

    with tempfile.TemporaryDirectory() as tmp:
        tmpdir = Path(tmp)
        for check in checks:
            if check(binary, root, tmpdir):
                passed += 1

    print()
    print(f" - Passed: {passed} out of {total}")
    return 0 if passed == total else 1
