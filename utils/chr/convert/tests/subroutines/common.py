from __future__ import annotations

import subprocess

from pathlib import Path
from typing import Dict, List, Optional, Tuple

from tests.helpers.output import print_indented
from tests.helpers.png_rgb import Image, Rgb, read_png_rgb
from tests.helpers.runner import run


COMPONENT_COLORS_SIZE = (152, 64)
COMPONENT_PALETTES_SIZE = (152, 64)
COMPONENT_TABLE_SIZE = (152, 160)

FULL_COLORS_OFFSET = (8, 176)
FULL_PALETTES_OFFSET = (168, 176)
FULL_LEFT_OFFSET = (8, 8)
FULL_RIGHT_OFFSET = (168, 8)

DATA_X = 16
DATA_Y = 24
CELL = 8
PALETTE_PAD = 1


def required_file(path: Path) -> Path:
    """Resolve a file path and require it to exist."""

    filename = path.resolve()
    if not filename.is_file():
        raise RuntimeError(f"File {filename} not found")
    return filename


def components_root(root: Path) -> Path:
    """Return the chr-convert component fixtures root."""

    return root / "utils/chr/convert/tests/components"


def golden_root(root: Path) -> Path:
    """Return the chr-convert golden files root."""

    return root / "utils/chr/convert/tests/golden"


def template_2c02(root: Path) -> Path:
    """Return the shared full 2C02 template fixture."""

    return required_file(
        root / "utils/chr/convert/templates/user/2c02-template.png"
    )


def fixture(root: Path, name: str) -> Path:
    """Return a component fixture path."""

    return required_file(components_root(root) / name)


def run_command(
    program: Path,
    args: List[str]
) -> subprocess.CompletedProcess[str]:
    """Run chr-convert and capture output."""

    return run(
        [str(program), *args],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )


def print_process_output(proc: subprocess.CompletedProcess[str]) -> None:
    """Print captured process output for a failed test."""

    if proc.stdout:
        print_indented("stdout:")
        print_indented(proc.stdout.rstrip())
    if proc.stderr:
        print_indented("stderr:")
        print_indented(proc.stderr.rstrip())


def check_success(
    name: str,
    proc: subprocess.CompletedProcess[str]
) -> Optional[str]:
    """Return an error message if the command did not succeed cleanly."""

    if proc.returncode != 0:
        return f"exit code {proc.returncode}"
    if proc.stdout.strip() or proc.stderr.strip():
        return "unexpected output"
    return None


def read_colors(
    image: Image,
    origin: Tuple[int, int]
) -> Tuple[List[Rgb], List[bool]]:
    """Read the PPU colors table from a component or full image."""

    ox, oy = origin
    colors: List[Rgb] = []
    allowed: List[bool] = []

    for y in range(4):
        for x in range(16):
            px = ox + DATA_X + x * CELL
            py = oy + DATA_Y + y * CELL
            color = image[py][px]
            colors.append(color)
            allowed.append(
                all(
                    image[py + dy][px + dx] == color
                    for dy in range(CELL)
                    for dx in range(CELL)
                )
            )

    return colors, allowed


def _lookup_color(colors: List[Rgb], allowed: List[bool], color: Rgb) -> int:
    """Find a color in the PPU colors table."""

    for index, candidate in enumerate(colors):
        if allowed[index] and candidate == color:
            return index

    raise RuntimeError(f"Color {color!r} is not present in PPU colors")


def read_palette_colors(
    image: Image,
    origin: Tuple[int, int]
) -> List[List[List[Rgb]]]:
    """Read palette RGB groups from a component or full image."""

    ox, oy = origin
    result: List[List[List[Rgb]]] = []

    for plane in range(2):
        plane_groups: List[List[Rgb]] = []
        for group in range(4):
            group_colors: List[Rgb] = []
            for color in range(4):
                px = ox + DATA_X + (group * 4 + color) * CELL + PALETTE_PAD
                py = oy + DATA_Y + plane * CELL + PALETTE_PAD
                group_colors.append(image[py][px])
            plane_groups.append(group_colors)
        result.append(plane_groups)

    return result


def palette_indexes(
    ppu_colors: List[Rgb],
    allowed: List[bool],
    palettes: List[List[List[Rgb]]]
) -> bytes:
    """Encode palette RGB groups as .spals bytes."""

    data = bytearray()
    for plane in palettes:
        for group in plane:
            for color in group:
                data.append(_lookup_color(ppu_colors, allowed, color))
    return bytes(data)


def read_table_pixels(
    image: Image,
    origin: Tuple[int, int]
) -> List[List[List[List[Rgb]]]]:
    """Read pattern table tile pixels from a component or full image."""

    ox, oy = origin
    table: List[List[List[List[Rgb]]]] = []

    for ty in range(16):
        row: List[List[List[Rgb]]] = []
        for tx in range(16):
            tile: List[List[Rgb]] = []
            for py in range(8):
                tile_row: List[Rgb] = []
                for px in range(8):
                    tile_row.append(
                        image[
                            oy + DATA_Y + ty * CELL + py
                        ][
                            ox + DATA_X + tx * CELL + px
                        ]
                    )
                tile.append(tile_row)
            row.append(tile)
        table.append(row)

    return table


def _unique_tile_colors(tile: List[List[Rgb]]) -> List[Rgb]:
    """Return unique tile colors in read order."""

    result: List[Rgb] = []
    for row in tile:
        for color in row:
            if color not in result:
                result.append(color)
    return result


def _select_palette_group(
    tile: List[List[Rgb]],
    groups: List[List[Rgb]],
    explicit: Optional[int] = None
) -> int:
    """Select the palette group for one tile."""

    tile_colors = _unique_tile_colors(tile)

    if explicit is not None:
        for color in tile_colors:
            if color not in groups[explicit]:
                raise RuntimeError(
                    "Explicit palette does not contain tile color"
                )
        return explicit

    matches = [
        index
        for index, group in enumerate(groups)
        if all(color in group for color in tile_colors)
    ]
    if not matches:
        raise RuntimeError("Cannot find matching palette for tile")

    selected = matches[0]
    for match in matches[1:]:
        for color in tile_colors:
            for position, candidate in enumerate(groups[selected]):
                if candidate == color:
                    if groups[match][position] != color:
                        raise RuntimeError("Ambiguous palette selection")
                    break

    return selected


def _tile_chr_bytes(tile: List[List[Rgb]], group: List[Rgb]) -> bytes:
    """Encode one tile as 16 CHR bytes."""

    planes = [[0] * 8, [0] * 8]

    for y, row in enumerate(tile):
        for x, color in enumerate(row):
            index = group.index(color)
            shift = 7 - x
            planes[0][y] |= ((index >> 0) & 1) << shift
            planes[1][y] |= ((index >> 1) & 1) << shift

    return bytes(planes[0] + planes[1])


def pattern_table_bytes(
    table: List[List[List[List[Rgb]]]],
    palette_groups: List[List[Rgb]],
    explicit: Optional[Dict[Tuple[int, int], int]] = None
) -> bytes:
    """Encode one pattern table as .pat bytes."""

    data = bytearray()
    explicit = explicit or {}

    for ty, row in enumerate(table):
        for tx, tile in enumerate(row):
            group_index = _select_palette_group(
                tile,
                palette_groups,
                explicit.get((tx, ty))
            )
            data.extend(_tile_chr_bytes(tile, palette_groups[group_index]))

    return bytes(data)


def expected_from_images(
    colors_image: Image,
    palettes_image: Image,
    left_image: Image,
    right_image: Image,
    origins: Tuple[
        Tuple[int, int],
        Tuple[int, int],
        Tuple[int, int],
        Tuple[int, int]
    ] = ((0, 0), (0, 0), (0, 0), (0, 0))
) -> Tuple[bytes, bytes, bytes]:
    """Build expected .pal, .spals and .pats bytes from PNG images."""

    colors_origin, palettes_origin, left_origin, right_origin = origins
    ppu_colors, allowed = read_colors(colors_image, colors_origin)
    palettes = read_palette_colors(palettes_image, palettes_origin)
    left = read_table_pixels(left_image, left_origin)
    right = read_table_pixels(right_image, right_origin)

    pal = bytes(value for color in ppu_colors for value in color)
    spals = palette_indexes(ppu_colors, allowed, palettes)
    pats = (
        pattern_table_bytes(left, palettes[0]) +
        pattern_table_bytes(right, palettes[1])
    )

    return pal, spals, pats


def expected_from_components(root: Path) -> Tuple[bytes, bytes, bytes]:
    """Build expected bytes from the component fixtures."""

    return expected_from_images(
        read_png_rgb(fixture(root, "colors.png")),
        read_png_rgb(fixture(root, "palettes0.png")),
        read_png_rgb(fixture(root, "table0.png")),
        read_png_rgb(fixture(root, "table1.png"))
    )


def expected_from_full_template(path: Path) -> Tuple[bytes, bytes, bytes]:
    """Build expected bytes from a full template PNG."""

    image = read_png_rgb(path)
    return expected_from_images(
        image,
        image,
        image,
        image,
        (
            FULL_COLORS_OFFSET,
            FULL_PALETTES_OFFSET,
            FULL_LEFT_OFFSET,
            FULL_RIGHT_OFFSET
        )
    )


def first_diff(expected: bytes, actual: bytes) -> str:
    """Return a compact byte-level diff summary."""

    for i, expected_byte in enumerate(expected):
        if i >= len(actual):
            return f"actual ended at byte {i}"
        if actual[i] != expected_byte:
            return (
                f"byte {i}: got 0x{actual[i]:02x}, "
                f"expected 0x{expected_byte:02x}"
            )

    if len(actual) != len(expected):
        return f"actual has {len(actual) - len(expected)} extra bytes"

    return "unknown byte difference"
