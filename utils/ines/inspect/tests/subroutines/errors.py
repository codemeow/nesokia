from __future__ import annotations

import json
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


def error_cases(
    repo_root: Path,
    invalid_rom: Path,
    missing_rom: Path
) -> List[SemanticCase]:
    """Build the inspect error handling test cases list."""

    shared_cases = nes_cases(repo_root)
    first_rom = shared_cases[0].reference.relative_to(repo_root)
    second_rom = shared_cases[1].reference.relative_to(repo_root)

    valid_paths = [
        str(first_rom),
        str(second_rom)
    ]

    return [
        SemanticCase(
            section="Error Handling",
            name="invalid ROM is skipped",
            args=[
                "-o",
                "json",
                valid_paths[0],
                str(invalid_rom),
                valid_paths[1]
            ],
            cwd=repo_root,
            check=check_skipped_path(
                valid_paths,
                str(invalid_rom),
                "is not a iNES/NES2.0 file"
            )
        ),
        SemanticCase(
            section="Error Handling",
            name="missing file is skipped",
            args=[
                "-o",
                "json",
                valid_paths[0],
                str(missing_rom),
                valid_paths[1]
            ],
            cwd=repo_root,
            check=check_skipped_path(
                valid_paths,
                str(missing_rom),
                "cannot `lstat` file"
            )
        )
    ]


def check_skipped_path(
    expected_files: List[str],
    skipped_path: str,
    stderr_fragment: str
) -> Any:
    """Build a check that one broken path is reported and skipped."""

    def check(proc: Any) -> None:
        entries = json.loads(proc.stdout)
        actual_files = [
            entry["filename"]
            for entry in entries
        ]

        if actual_files != expected_files:
            raise AssertionError(
                f"files: got {actual_files!r}, expected {expected_files!r}"
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


def run_error_tests(root: Path, program: Path) -> int:
    """Run the inspect error handling tests."""

    with tempfile.TemporaryDirectory() as tmp:
        tmpdir = Path(tmp)
        invalid_rom = tmpdir / "invalid.nes"
        missing_rom = tmpdir / "missing.nes"

        invalid_rom.write_bytes(b"not an ines rom!")

        return run_semantic_cases(
            required_file(program),
            error_cases(root, invalid_rom, missing_rom)
        )
