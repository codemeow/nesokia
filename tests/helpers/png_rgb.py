from __future__ import annotations

import struct
import zlib

from pathlib import Path
from typing import List, Tuple


Rgb = Tuple[int, int, int]
Image = List[List[Rgb]]


def _paeth(a: int, b: int, c: int) -> int:
    """Return the PNG Paeth predictor."""

    p = a + b - c
    pa = abs(p - a)
    pb = abs(p - b)
    pc = abs(p - c)

    if pa <= pb and pa <= pc:
        return a
    if pb <= pc:
        return b
    return c


def read_png_rgb(path: Path) -> Image:
    """Read an 8-bit RGB/RGBA PNG into RGB rows."""

    data = path.read_bytes()
    if not data.startswith(b"\x89PNG\r\n\x1a\n"):
        raise RuntimeError(f"{path} is not a PNG file")

    pos = 8
    width = 0
    height = 0
    color_type = 0
    compressed = b""

    while pos < len(data):
        size = struct.unpack(">I", data[pos:pos + 4])[0]
        kind = data[pos + 4:pos + 8]
        payload = data[pos + 8:pos + 8 + size]
        pos += 12 + size

        if kind == b"IHDR":
            width, height, bit_depth, color_type, _, _, interlace = (
                struct.unpack(">IIBBBBB", payload)
            )
            if bit_depth != 8:
                raise RuntimeError(f"{path} uses unsupported PNG bit depth")
            if color_type not in (2, 6):
                raise RuntimeError(f"{path} uses unsupported PNG color type")
            if interlace != 0:
                raise RuntimeError(f"{path} uses unsupported PNG interlace")
        elif kind == b"IDAT":
            compressed += payload
        elif kind == b"IEND":
            break

    channels = 3 if color_type == 2 else 4
    bpp = channels
    stride = width * channels
    raw = zlib.decompress(compressed)
    rows = []
    previous = [0] * stride
    offset = 0

    for _ in range(height):
        filter_type = raw[offset]
        offset += 1
        scanline = list(raw[offset:offset + stride])
        offset += stride

        decoded = [0] * stride
        for i, value in enumerate(scanline):
            left = decoded[i - bpp] if i >= bpp else 0
            up = previous[i]
            upper_left = previous[i - bpp] if i >= bpp else 0

            if filter_type == 0:
                decoded[i] = value
            elif filter_type == 1:
                decoded[i] = (value + left) & 0xff
            elif filter_type == 2:
                decoded[i] = (value + up) & 0xff
            elif filter_type == 3:
                decoded[i] = (value + ((left + up) // 2)) & 0xff
            elif filter_type == 4:
                decoded[i] = (value + _paeth(left, up, upper_left)) & 0xff
            else:
                raise RuntimeError(f"{path} uses unsupported PNG filter")

        row = []
        for x in range(width):
            base = x * channels
            row.append(tuple(decoded[base:base + 3]))
        rows.append(row)
        previous = decoded

    return rows
