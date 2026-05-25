from __future__ import annotations

import json
import shutil
import tempfile

from pathlib import Path
from typing import Any, List, Union

from tests.helpers.nes_cases import nes_cases
from tests.helpers.semantic import SemanticCase, run_semantic_cases


def required_file(path: Union[str, Path]) -> Path:
    """Resolve a file path and require it to exist."""

    filename = Path(path).resolve()
    if not filename.is_file():
        raise RuntimeError(f"File {filename} not found")
    return filename


def directory_cases(
    repo_root: Path,
    scan_root: Path,
    empty_root: Path,
    missing_root: Path,
    expected_files: List[str],
    skipped_file: Path
) -> List[SemanticCase]:
    """Build the inspect directory scan test cases list."""

    return [
        SemanticCase(
            section="Directory Scan",
            name="recursive scan skips broken files",
            args=[
                "-r",
                "-o",
                "json",
                str(scan_root)
            ],
            cwd=repo_root,
            check=check_directory_scan(
                expected_files,
                str(skipped_file),
                "cannot read file"
            )
        ),
        SemanticCase(
            section="Directory Scan",
            name="missing directory is fatal",
            args=[
                "-r",
                "-o",
                "json",
                str(missing_root)
            ],
            cwd=repo_root,
            expected_code=1,
            check=check_fatal_directory(str(missing_root))
        ),
        SemanticCase(
            section="Directory Scan",
            name="empty directory is fatal",
            args=[
                "-r",
                "-o",
                "json",
                str(empty_root)
            ],
            cwd=repo_root,
            expected_code=1,
            check=check_fatal_directory(str(empty_root))
        )
    ]


def check_directory_scan(
    expected_files: List[str],
    skipped_path: str,
    stderr_fragment: str
) -> Any:
    """Build a check for recursive directory scan behavior."""

    def check(proc: Any) -> None:
        entries = json.loads(proc.stdout)
        actual_files = {
            entry["filename"]
            for entry in entries
        }

        expected = set(expected_files)
        if actual_files != expected:
            raise AssertionError(
                f"files: got {sorted(actual_files)!r}, "
                f"expected {sorted(expected)!r}"
            )

        if skipped_path in proc.stdout:
            raise AssertionError(
                f"stdout contains skipped path {skipped_path!r}"
            )

        if skipped_path not in proc.stderr:
            raise AssertionError(f"stderr does not contain {skipped_path!r}")

        if stderr_fragment not in proc.stderr:
            raise AssertionError(
                f"stderr does not contain {stderr_fragment!r}"
            )

    return check


def check_fatal_directory(path: str) -> Any:
    """Build a check for directory scans without valid ROM files."""

    def check(proc: Any) -> None:
        if proc.stdout:
            raise AssertionError("stdout is not empty")

        if "Error: no valid ROM files found" not in proc.stderr:
            raise AssertionError("stderr does not contain no-valid-ROM error")

        if not Path(path).exists() and path not in proc.stderr:
            raise AssertionError(f"stderr does not contain {path!r}")

    return check


def prepare_scan_root(
    repo_root: Path,
    tmpdir: Path
) -> tuple[Path, Path, Path, List[str]]:
    """Create a temporary directory tree for recursive scan tests."""

    shared_cases = nes_cases(repo_root)
    scan_root = tmpdir / "scan"
    nested_root = scan_root / "nested"
    empty_root = tmpdir / "empty"

    nested_root.mkdir(parents=True)
    empty_root.mkdir()

    root_rom = scan_root / "root-rom.bin"
    nested_rom = nested_root / "nested-rom.data"
    skipped_file = scan_root / "readme.txt"

    shutil.copyfile(shared_cases[0].reference, root_rom)
    shutil.copyfile(shared_cases[1].reference, nested_rom)
    skipped_file.write_text("not a rom\n", encoding="utf-8")

    expected_files = [
        str(root_rom),
        str(nested_rom)
    ]

    return scan_root, empty_root, skipped_file, expected_files


def run_directory_tests(root: Path, program: Path) -> int:
    """Run the inspect directory scan tests."""

    with tempfile.TemporaryDirectory() as tmp:
        tmpdir = Path(tmp)
        missing_root = tmpdir / "missing"

        scan_root, empty_root, skipped_file, expected_files = (
            prepare_scan_root(root, tmpdir)
        )

        return run_semantic_cases(
            required_file(program),
            directory_cases(
                root,
                scan_root,
                empty_root,
                missing_root,
                expected_files,
                skipped_file
            )
        )
