from __future__ import annotations

import shutil
import subprocess

from pathlib import Path
from typing import List


def _imagemagick_command() -> List[str]:
    """Return the available ImageMagick command prefix."""

    if shutil.which("magick"):
        return ["magick"]

    if shutil.which("convert"):
        return ["convert"]

    raise RuntimeError(
        "ImageMagick is required for PNG compose tests: "
        "install `magick` or `convert`"
    )


def compose_chr_template(
    output: Path,
    colors: Path,
    palettes: Path,
    left: Path,
    right: Path
) -> None:
    """Compose a full CHR template PNG from component PNG files."""

    cmd = [
        *_imagemagick_command(),
        "-size",
        "328x248",
        "xc:rgb(236,238,236)",
        str(left),
        "-geometry",
        "+8+8",
        "-composite",
        str(right),
        "-geometry",
        "+168+8",
        "-composite",
        str(colors),
        "-geometry",
        "+8+176",
        "-composite",
        str(palettes),
        "-geometry",
        "+168+176",
        "-composite",
        str(output)
    ]

    proc = subprocess.run(
        cmd,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    if proc.returncode != 0:
        raise RuntimeError(
            "Cannot compose CHR template PNG with ImageMagick:\n"
            f"{proc.stderr.rstrip()}"
        )
