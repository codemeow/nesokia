from __future__ import annotations

from pathlib import Path
from typing import Union


def required_file(path: Union[str, Path]) -> Path:
    """Resolve a file path and require it to exist."""

    filename = Path(path).resolve()
    if not filename.is_file():
        raise RuntimeError(f"File {filename} not found")
    return filename


def template_2c02(root: Path) -> Path:
    """Return the shared 2C02 PAL fixture."""

    return required_file(root / "utils/chr/colors/templates/pal/2c02.pal")


def golden_root(root: Path) -> Path:
    """Return the chr-colors golden files root."""

    return root / "utils/chr/colors/tests/golden"
