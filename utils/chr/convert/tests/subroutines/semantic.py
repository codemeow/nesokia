from __future__ import annotations

import subprocess
import tempfile

from pathlib import Path
from typing import Callable, List

from tests.helpers.output import print_failed, print_indented, print_passed
from tests.helpers.png_compose import compose_chr_template

from subroutines.common import (
    expected_from_components,
    first_diff,
    fixture,
    print_process_output,
    required_file,
    run_command,
)


def _compare_file(name: str, expected: bytes, actual: Path) -> bool:
    """Compare one file to expected bytes."""

    actual_bytes = actual.read_bytes()
    if actual_bytes != expected:
        print_failed(name, "output differs")
        print_indented(first_diff(expected, actual_bytes))
        return False

    return True


def _check_success(name: str, proc: subprocess.CompletedProcess[str]) -> bool:
    """Check successful process completion."""

    if proc.returncode != 0:
        print_failed(name, f"exit code {proc.returncode}")
        print_process_output(proc)
        return False

    if proc.stdout.strip() or proc.stderr.strip():
        print_failed(name, "unexpected output")
        print_process_output(proc)
        return False

    return True


def run_components_expected(program: Path, root: Path, tmpdir: Path) -> bool:
    """Check that component input produces independently expected bytes."""

    expected_pal, expected_spals, expected_pats = (
        expected_from_components(root)
    )
    pal = tmpdir / "components.pal"
    spals = tmpdir / "components.spals"
    pats = tmpdir / "components.pats"

    proc = run_command(
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
            "-C",
            str(pal),
            "-P",
            str(spals),
            "-T",
            str(pats)
        ]
    )
    if not _check_success("components expected bytes", proc):
        return False

    if not _compare_file("components colors", expected_pal, pal):
        return False
    if not _compare_file("components palettes", expected_spals, spals):
        return False
    if not _compare_file("components pattern tables", expected_pats, pats):
        return False

    print_passed("components expected bytes")
    return True


def run_full_vs_components(program: Path, root: Path, tmpdir: Path) -> bool:
    """Check that composed full input matches separate component input."""

    full = tmpdir / "full.png"
    compose_chr_template(
        full,
        fixture(root, "colors.png"),
        fixture(root, "palettes0.png"),
        fixture(root, "table0.png"),
        fixture(root, "table1.png")
    )

    component_pats = tmpdir / "component.pats"
    full_pats = tmpdir / "full.pats"

    proc = run_command(
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
            str(component_pats)
        ]
    )
    if not _check_success("components binary output", proc):
        return False

    proc = run_command(
        program,
        ["-q", "-i", str(full), "-T", str(full_pats)]
    )
    if not _check_success("full binary output", proc):
        return False

    if component_pats.read_bytes() != full_pats.read_bytes():
        print_failed("full vs components", "pattern tables differ")
        print_indented(
            first_diff(component_pats.read_bytes(), full_pats.read_bytes())
        )
        return False

    print_passed("full vs components")
    return True


def run_roundtrip(program: Path, root: Path, tmpdir: Path) -> bool:
    """Check template -> binaries -> template -> binaries."""

    full = tmpdir / "full.png"
    compose_chr_template(
        full,
        fixture(root, "colors.png"),
        fixture(root, "palettes0.png"),
        fixture(root, "table0.png"),
        fixture(root, "table1.png")
    )

    pal = tmpdir / "roundtrip.pal"
    spals = tmpdir / "roundtrip.spals"
    pats = tmpdir / "roundtrip.pats"
    full2 = tmpdir / "roundtrip-full.png"
    pal2 = tmpdir / "roundtrip2.pal"
    spals2 = tmpdir / "roundtrip2.spals"
    pats2 = tmpdir / "roundtrip2.pats"

    proc = run_command(
        program,
        [
            "-q",
            "-i",
            str(full),
            "-C",
            str(pal),
            "-P",
            str(spals),
            "-T",
            str(pats)
        ]
    )
    if not _check_success("roundtrip first t2c", proc):
        return False

    proc = run_command(
        program,
        [
            "-q",
            "-M",
            "c2t",
            "-c",
            str(pal),
            "-p",
            str(spals),
            "-t",
            str(pats),
            "-O",
            str(full2)
        ]
    )
    if not _check_success("roundtrip c2t", proc):
        return False

    proc = run_command(
        program,
        [
            "-q",
            "-i",
            str(full2),
            "-C",
            str(pal2),
            "-P",
            str(spals2),
            "-T",
            str(pats2)
        ]
    )
    if not _check_success("roundtrip second t2c", proc):
        return False

    if pal.read_bytes() != pal2.read_bytes():
        print_failed("template roundtrip", "colors differ")
        return False
    if spals.read_bytes() != spals2.read_bytes():
        print_failed("template roundtrip", "palettes differ")
        return False
    if pats.read_bytes() != pats2.read_bytes():
        print_failed("template roundtrip", "pattern tables differ")
        return False

    print_passed("template to binaries to template to binaries")
    return True


def run_explicit_palettes(program: Path, root: Path, tmpdir: Path) -> bool:
    """Check explicit-palettes success and failure paths."""

    base_args = [
        "-q",
        "-c",
        str(fixture(root, "colors.png")),
        "-p",
        str(fixture(root, "palettesE.png")),
        "-l",
        str(fixture(root, "tableE.png")),
        "-r",
        str(fixture(root, "table1.png"))
    ]

    proc = run_command(
        program,
        [*base_args, "-T", str(tmpdir / "ambiguous.pats")]
    )
    if proc.returncode == 0:
        print_failed("explicit required", "command unexpectedly succeeded")
        return False
    if "explicit" not in proc.stderr:
        print_failed("explicit required", "stderr does not mention explicit")
        print_process_output(proc)
        return False
    print_passed("explicit required")

    proc = run_command(
        program,
        [
            *base_args,
            "-e",
            "l00=1",
            "-T",
            str(tmpdir / "explicit-ok.pats")
        ]
    )
    if not _check_success("explicit correct", proc):
        return False
    print_passed("explicit correct")

    proc = run_command(
        program,
        [
            *base_args,
            "-e",
            "l00=2",
            "-T",
            str(tmpdir / "explicit-bad.pats")
        ]
    )
    if proc.returncode == 0:
        print_failed("explicit wrong", "command unexpectedly succeeded")
        return False
    if "does not contain" not in proc.stderr:
        print_failed("explicit wrong", "stderr does not mention bad palette")
        print_process_output(proc)
        return False
    print_passed("explicit wrong")
    return True


def run_semantic_tests(root: Path, program: Path) -> int:
    """Run the chr-convert semantic tests."""

    binary = required_file(program)
    checks: List[Callable[[Path, Path, Path], bool]] = [
        run_components_expected,
        run_full_vs_components,
        run_roundtrip,
        run_explicit_palettes
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
