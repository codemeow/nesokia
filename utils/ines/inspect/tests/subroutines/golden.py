from __future__ import annotations

from pathlib import Path
from typing import List, Union

from tests.helpers.golden import GoldenCase, run_golden_cases
from tests.helpers.nes_cases import nes_cases


FORMATS = [
    "tree",
    "table",
    "json",
    "xml",
    "yaml",
    "md",
    "csv"
]

CASE_INDEXES = [
    "00",
    "02",
    "16",
    "27",
    "30"
]


def required_file(path: Union[str, Path]) -> Path:
    """Resolve a file path and require it to exist."""

    filename = Path(path).resolve()
    if not filename.is_file():
        raise RuntimeError(f"File {filename} not found")
    return filename


def golden_root(project_root: Path) -> Path:
    """Return the inspect golden files root."""

    return project_root / "utils/ines/inspect/tests/golden"


def case_by_index(repo_root: Path) -> dict[str, Path]:
    """Return shared NES ROM paths indexed by fixture prefix."""

    return {
        case.name[:2]: case.reference.relative_to(repo_root)
        for case in nes_cases(repo_root)
    }


def golden_cases(repo_root: Path) -> List[GoldenCase]:
    """Build the inspect golden output test cases list."""

    selected_cases = case_by_index(repo_root)
    cases: List[GoldenCase] = []

    for output_format in FORMATS:
        for index in CASE_INDEXES:
            expected = golden_root(repo_root) / output_format / f"{index}.out"
            cases.append(
                GoldenCase(
                    section=output_format,
                    name=index,
                    args=[
                        "-o",
                        output_format,
                        str(selected_cases[index])
                    ],
                    expected=expected,
                    cwd=repo_root
                )
            )

    return cases


def run_golden_tests(root: Path, program: Path) -> int:
    """Run the inspect golden output tests."""

    return run_golden_cases(required_file(program), golden_cases(root))
