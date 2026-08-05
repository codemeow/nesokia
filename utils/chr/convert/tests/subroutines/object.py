from __future__ import annotations

import tempfile

from pathlib import Path
from typing import List, Tuple

from tests.helpers.output import print_failed, print_indented, print_passed

from subroutines.common import first_diff, print_process_output, required_file, run_command


def _root(root: Path) -> Path:
    """Return the object fixtures directory."""

    return root / "utils/chr/convert/tests/object"


def _fixture(root: Path, name: str) -> Path:
    """Return one required object fixture."""

    return required_file(_root(root) / name)


def _base_args(root: Path, object_name: str, palette_name: str) -> List[str]:
    """Build the shared object conversion command arguments."""

    return [
        "-q",
        "-o", str(_fixture(root, object_name)),
        "-c", str(_fixture(root, "colors.png")),
        "-p", str(_fixture(root, palette_name)),
    ]


def _outputs(tmpdir: Path, name: str) -> Tuple[Path, Path, List[str]]:
    """Build output paths and their CLI arguments."""

    pat = tmpdir / f"{name}.pat"
    atr = tmpdir / f"{name}.atr"
    return pat, atr, ["-D", str(pat), "-G", str(atr)]


def _success(name: str, proc: object) -> bool:
    """Check a conversion process completed silently and successfully."""

    if proc.returncode != 0:
        print_failed(name, f"exit code {proc.returncode}")
        print_process_output(proc)
        return False
    if proc.stdout.strip() or proc.stderr.strip():
        print_failed(name, "unexpected output")
        print_process_output(proc)
        return False
    return True


def _same_file(name: str, expected: Path, actual: Path) -> bool:
    """Compare a produced binary file to the fixed independently made golden."""

    actual_data = actual.read_bytes()
    expected_data = expected.read_bytes()
    if actual_data != expected_data:
        print_failed(name, "output differs from golden")
        print_indented(first_diff(expected_data, actual_data))
        return False
    return True


def _valid_case(
    program: Path,
    root: Path,
    tmpdir: Path,
    title: str,
    object_name: str,
    palette_name: str = "palettes.png"
) -> bool:
    """Run one valid object fixture against static .pat and .atr goldens."""

    pat, atr, output_args = _outputs(tmpdir, title)
    proc = run_command(
        program,
        [
            *_base_args(root, object_name, palette_name),
            *output_args,
        ]
    )
    if not _success(title, proc):
        return False
    if not _same_file(f"{title} .pat", _fixture(root, object_name.replace(".png", ".pat")), pat):
        return False
    if not _same_file(f"{title} .atr", _fixture(root, object_name.replace(".png", ".atr")), atr):
        return False

    print_passed(title)
    return True


def _invalid_dimensions(program: Path, root: Path, tmpdir: Path) -> bool:
    """Require odd tile dimensions to be rejected before conversion."""

    names = ("invalid-4x3.png", "invalid-3x4.png", "invalid-3x3.png")
    for name in names:
        _, _, output_args = _outputs(tmpdir, name)
        proc = run_command(
            program,
            [*_base_args(root, name, "palettes.png"), *output_args]
        )
        if proc.returncode == 0 or "dimensions must be multiples" not in proc.stderr:
            print_failed(f"{name} dimensions", "invalid object was accepted")
            print_process_output(proc)
            return False

    print_passed("invalid odd tile dimensions")
    return True


def _ambiguous_palette(program: Path, root: Path, tmpdir: Path) -> bool:
    """Select the first matching palette for an ambiguous background group."""

    pat, atr, output_args = _outputs(tmpdir, "ambiguous")
    base = _base_args(root, "ambiguous-2x2.png", "palettes-ambiguous.png")
    proc = run_command(program, [*base, *output_args])
    if not _success("object first matching palette", proc):
        return False
    if not _same_file("object first matching .pat", _fixture(root, "ambiguous-2x2.pat"), pat):
        return False
    if not _same_file("object first matching .atr", _fixture(root, "ambiguous-2x2.atr"), atr):
        return False

    proc = run_command(program, [*base, "-e", "l00=0", *output_args])
    if proc.returncode == 0 or "cannot be used in object mode" not in proc.stderr:
        print_failed("object rejects explicit palettes", "-e was accepted")
        print_process_output(proc)
        return False

    print_passed("object first matching palette")
    return True


def _background_spal(program: Path, root: Path, tmpdir: Path) -> bool:
    """Check the object mode accepts the separate binary background palette."""

    pat, atr, output_args = _outputs(tmpdir, "background-spal")
    proc = run_command(
        program,
        [
            "-q",
            "-o", str(_fixture(root, "lynx-2x2.png")),
            "-c", str(_fixture(root, "colors.png")),
            "-b", str(_fixture(root, "background.spal")),
            *output_args,
        ]
    )
    if not _success("object background .spal", proc):
        return False
    if not _same_file("object background .spal .pat", _fixture(root, "lynx-2x2.pat"), pat):
        return False
    if not _same_file("object background .spal .atr", _fixture(root, "lynx-2x2.atr"), atr):
        return False

    print_passed("object background .spal")
    return True


def run_object_tests(root: Path, program: Path) -> int:
    """Run object PNG tests against separately generated fixed goldens."""

    binary = required_file(program)
    checks = (
        lambda tmp: _valid_case(binary, root, tmp, "object 4x4 tiles", "window-4x4.png"),
        lambda tmp: _valid_case(binary, root, tmp, "object 8x6 tiles", "lighthouse-8x6.png"),
        lambda tmp: _valid_case(binary, root, tmp, "object 2x2 tiles", "lynx-2x2.png"),
        lambda tmp: _valid_case(binary, root, tmp, "object wider than 256 pixels", "wide-34x2.png"),
        lambda tmp: _invalid_dimensions(binary, root, tmp),
        lambda tmp: _ambiguous_palette(binary, root, tmp),
        lambda tmp: _background_spal(binary, root, tmp),
    )

    print("# Object")
    passed = 0
    with tempfile.TemporaryDirectory() as directory:
        tmpdir = Path(directory)
        for check in checks:
            if check(tmpdir):
                passed += 1

    print()
    print(f" - Passed: {passed} out of {len(checks)}")
    return 0 if passed == len(checks) else 1
