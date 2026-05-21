from dataclasses import dataclass
from pathlib import Path
from typing import List, Union


@dataclass(frozen=True)
class NesCase:
    name     : str  ##< Test case name
    root     : Path ##< Path to the test case directory
    meta     : Path ##< Path to the NES20DB slice
    reference: Path ##< Path to the reference ROM


def resolve_dir(path: Union[str, Path]) -> Path:
    directory = Path(path).resolve()
    if not directory.is_dir():
        raise RuntimeError(f"Directory {directory} not found")
    return directory


def resolve_file(path: Union[str, Path]) -> Path:
    file = Path(path).resolve()
    if not file.is_file():
        raise RuntimeError(f"File {file} not found")
    return file


def nes_cases_root(repo_root: Union[str, Path]) -> Path:
    return resolve_dir(Path(repo_root) / "tests/nes")


def nes_cases(repo_root: Union[str, Path]) -> List[NesCase]:
    cases: List[NesCase] = []

    for root in sorted(nes_cases_root(repo_root).iterdir()):
        if not root.is_dir():
            continue

        cases.append(
            NesCase(
                name      = root.name,
                root      = root,
                meta      = resolve_file(root / "nes20db-slice.xml"),
                reference = resolve_file(root / "rom-header.nes")
            )
        )

    return cases
