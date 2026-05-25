from dataclasses import dataclass
from pathlib import Path
from typing import Optional, Union
from xml.etree import ElementTree


@dataclass(frozen=True)
class Nes20DbHeader:
    """Normalized header fields from a nes20db XML slice."""

    isnes20: bool
    console: int
    region: int
    vs_ppu: Optional[int]
    vs_hardware: Optional[int]
    mirroring: int
    alternative: int
    mapper: int
    submapper: int
    prg_rom: int
    chr_rom: int
    prg_ram: int
    chr_ram: int
    prg_nvram: int
    chr_nvram: int
    misc_roms: int
    trainer: int
    battery: int
    device: int


def parse_slice(path: Union[str, Path]) -> Nes20DbHeader:
    """Parse a nes20db XML slice into normalized header fields."""

    game = ElementTree.parse(path).getroot().find("game")
    if game is None:
        raise RuntimeError(f"File {path} does not contain a game entry")

    pcb = _required_child(game, "pcb", path)
    console = _required_child(game, "console", path)
    expansion = _required_child(game, "expansion", path)
    vs = game.find("vs")

    mirroring = pcb.attrib.get("mirroring", "H")

    return Nes20DbHeader(
        isnes20=True,
        console=_attr_int(console, "type"),
        region=_attr_int(console, "region"),
        vs_ppu=_optional_nonzero_attr(vs, "ppu"),
        vs_hardware=_optional_nonzero_attr(vs, "hardware"),
        mirroring=_mirroring_raw(mirroring),
        alternative=_alternative_raw(mirroring),
        mapper=_attr_int(pcb, "mapper"),
        submapper=_attr_int(pcb, "submapper"),
        prg_rom=_child_size(game, "prgrom"),
        chr_rom=_child_size(game, "chrrom"),
        prg_ram=_child_size(game, "prgram"),
        chr_ram=_child_size(game, "chrram"),
        prg_nvram=_child_size(game, "prgnvram"),
        chr_nvram=_child_size(game, "chrnvram"),
        misc_roms=_child_count(game, "miscrom"),
        trainer=_child_present(game, "trainer"),
        battery=_attr_int(pcb, "battery"),
        device=_attr_int(expansion, "type")
    )


def _required_child(
    game: ElementTree.Element,
    name: str,
    path: Union[str, Path]
) -> ElementTree.Element:
    """Return a required child element."""

    child = game.find(name)
    if child is None:
        raise RuntimeError(f"File {path} does not contain a {name} entry")

    return child


def _attr_int(element: ElementTree.Element, name: str) -> int:
    """Return an integer attribute value."""

    return int(element.attrib[name])


def _optional_nonzero_attr(
    element: Optional[ElementTree.Element],
    name: str
) -> Optional[int]:
    """Return an optional integer attribute, mapping zero to None."""

    if element is None:
        return None

    value = _attr_int(element, name)
    return value if value != 0 else None


def _child_size(game: ElementTree.Element, name: str) -> int:
    """Return a child size attribute or zero if child is absent."""

    child = game.find(name)
    if child is None:
        return 0

    return _attr_int(child, "size")


def _child_count(game: ElementTree.Element, name: str) -> int:
    """Return the count attribute or zero if child is absent."""

    child = game.find(name)
    if child is None:
        return 0

    return _attr_int(child, "count")


def _child_present(game: ElementTree.Element, name: str) -> int:
    """Return one if child is present and zero otherwise."""

    return 1 if game.find(name) is not None else 0


def _mirroring_raw(value: str) -> int:
    """Convert nes20db mirroring notation to inspect raw mirroring."""

    return {
        "H": 0,
        "V": 1,
        "4": 0,
        "1": 1
    }[value]


def _alternative_raw(value: str) -> int:
    """Convert nes20db mirroring notation to inspect alternative flag."""

    return 1 if value in ("4", "1") else 0
