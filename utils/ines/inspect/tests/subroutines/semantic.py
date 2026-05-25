from __future__ import annotations

import json

from dataclasses import asdict
from pathlib import Path
from typing import Any, Dict, List, Union
from xml.etree import ElementTree

from tests.helpers.nes20db import Nes20DbHeader, parse_slice
from tests.helpers.nes_cases import NesCase, nes_cases
from tests.helpers.semantic import SemanticCase, run_semantic_cases


CASE_INDEXES = [
    "03",
    "11",
    "17",
    "20",
    "24",
    "26",
    "31"
]

JSON_FIELDS = {
    "isnes20": ("format", "isnes20"),
    "console": ("console", "console"),
    "region": ("console", "region"),
    "vs_ppu": ("console", "vs_ppu"),
    "vs_hardware": ("console", "vs_hardware"),
    "mirroring": ("nametables", "mirroring"),
    "alternative": ("nametables", "alternative"),
    "mapper": ("mapper", "mapper"),
    "submapper": ("mapper", "submapper"),
    "prg_rom": ("memory", "prg_rom"),
    "chr_rom": ("memory", "chr_rom"),
    "prg_ram": ("memory", "prg_ram"),
    "chr_ram": ("memory", "chr_ram"),
    "prg_nvram": ("memory", "prg_nvram"),
    "chr_nvram": ("memory", "chr_nvram"),
    "misc_roms": ("memory", "misc_roms"),
    "trainer": ("misc", "trainer"),
    "battery": ("misc", "battery"),
    "device": ("misc", "device")
}


def required_file(path: Union[str, Path]) -> Path:
    """Resolve a file path and require it to exist."""

    filename = Path(path).resolve()
    if not filename.is_file():
        raise RuntimeError(f"File {filename} not found")
    return filename


def case_by_index(repo_root: Path) -> Dict[str, NesCase]:
    """Return shared NES cases indexed by fixture prefix."""

    return {
        case.name[:2]: case
        for case in nes_cases(repo_root)
    }


def semantic_cases(repo_root: Path) -> List[SemanticCase]:
    """Build the inspect semantic test cases list."""

    selected_cases = case_by_index(repo_root)
    cases: List[SemanticCase] = []

    for index in CASE_INDEXES:
        case = selected_cases[index]
        cases.append(
            SemanticCase(
                section="JSON fields",
                name=case.name,
                args=[
                    "-o",
                    "json",
                    str(case.reference.relative_to(repo_root))
                ],
                cwd=repo_root,
                check=check_header_fields(parse_slice(case.meta))
            )
        )

    cases.extend(filter_cases(repo_root))
    cases.extend(keys_cases(repo_root))

    return cases


def check_header_fields(expected: Nes20DbHeader) -> Any:
    """Build a check that compares inspect JSON raw fields with XML data."""

    def check(proc: Any) -> None:
        entries = json.loads(proc.stdout)
        if len(entries) != 1:
            raise AssertionError(f"expected 1 JSON entry, got {len(entries)}")

        actual = raw_fields(entries[0])
        expected_fields = asdict(expected)

        for name, expected_value in expected_fields.items():
            actual_value = actual[name]
            if actual_value != expected_value:
                raise AssertionError(
                    f"{name}: got {actual_value!r}, "
                    f"expected {expected_value!r}"
                )

    return check


def raw_fields(entry: Dict[str, Any]) -> Dict[str, Any]:
    """Extract inspect JSON raw fields into a flat mapping."""

    return {
        name: entry[category][field]["raw"]
        for name, (category, field) in JSON_FIELDS.items()
    }


def filter_cases(repo_root: Path) -> List[SemanticCase]:
    """Build semantic filter checks."""

    cases = case_by_index(repo_root)
    selected = [
        cases[index]
        for index in CASE_INDEXES
    ]
    roms = [
        str(case.reference.relative_to(repo_root))
        for case in selected
    ]

    return [
        SemanticCase(
            section="Filter",
            name="mapper=99",
            args=["-o", "json", "-f", "mapper=99", *roms],
            cwd=repo_root,
            check=check_filtered_values("mapper", {99})
        ),
        SemanticCase(
            section="Filter",
            name="battery=1",
            args=["-o", "json", "-f", "battery=1", *roms],
            cwd=repo_root,
            check=check_filtered_values("battery", {1})
        ),
        SemanticCase(
            section="Filter",
            name="chr_rom=0",
            args=["-o", "json", "-f", "chr_rom=0", *roms],
            cwd=repo_root,
            check=check_filtered_values("chr_rom", {0})
        )
    ]


def check_filtered_values(field: str, expected_values: set[int]) -> Any:
    """Build a check that all JSON entries match expected raw values."""

    def check(proc: Any) -> None:
        entries = json.loads(proc.stdout)
        if not entries:
            raise AssertionError("expected at least one JSON entry")

        for entry in entries:
            value = raw_fields(entry)[field]
            if value not in expected_values:
                raise AssertionError(
                    f"{field}: got {value!r}, "
                    f"expected one of {sorted(expected_values)!r}"
                )

    return check


def keys_cases(repo_root: Path) -> List[SemanticCase]:
    """Build semantic keys checks."""

    case = case_by_index(repo_root)["24"]

    return [
        SemanticCase(
            section="Keys",
            name="json mapper and prg_rom only",
            args=[
                "-o",
                "json",
                "-k",
                "mapper&prg_rom",
                str(case.reference.relative_to(repo_root))
            ],
            cwd=repo_root,
            check=check_keys({"filename", "mapper", "memory"})
        ),
        SemanticCase(
            section="Keys",
            name="xml mapper and prg_rom only",
            args=[
                "-o",
                "xml",
                "-k",
                "mapper&prg_rom",
                str(case.reference.relative_to(repo_root))
            ],
            cwd=repo_root,
            check=check_xml_keys()
        )
    ]


def check_keys(expected_top_level: set[str]) -> Any:
    """Build a check that JSON output contains only expected top-level keys."""

    def check(proc: Any) -> None:
        entries = json.loads(proc.stdout)
        if len(entries) != 1:
            raise AssertionError(f"expected 1 JSON entry, got {len(entries)}")

        actual = set(entries[0].keys())
        if actual != expected_top_level:
            raise AssertionError(
                f"top-level keys: got {sorted(actual)!r}, "
                f"expected {sorted(expected_top_level)!r}"
            )

        mapper_keys = set(entries[0]["mapper"].keys())
        if mapper_keys != {"mapper"}:
            raise AssertionError(f"mapper keys: got {sorted(mapper_keys)!r}")

        memory_keys = set(entries[0]["memory"].keys())
        if memory_keys != {"prg_rom"}:
            raise AssertionError(f"memory keys: got {sorted(memory_keys)!r}")

    return check


def check_xml_keys() -> Any:
    """Build a check that XML output contains only selected fields."""

    def check(proc: Any) -> None:
        scan = ElementTree.fromstring(proc.stdout)
        games = scan.findall("game")
        if len(games) != 1:
            raise AssertionError(f"expected 1 XML game, got {len(games)}")

        game = games[0]

        pcb = game.find("pcb")
        if pcb is None:
            raise AssertionError("expected pcb element")
        if set(pcb.attrib.keys()) != {"mapper"}:
            raise AssertionError(
                f"pcb attributes: got {sorted(pcb.attrib.keys())!r}"
            )

        prgrom = game.find("prgrom")
        if prgrom is None:
            raise AssertionError("expected prgrom element")
        if set(prgrom.attrib.keys()) != {"size"}:
            raise AssertionError(
                f"prgrom attributes: got {sorted(prgrom.attrib.keys())!r}"
            )

        forbidden = [
            "format",
            "chrrom",
            "prgram",
            "chrram",
            "prgnvram",
            "chrnvram",
            "miscrom",
            "trainer",
            "console",
            "vs",
            "expansion"
        ]

        for name in forbidden:
            if game.find(name) is not None:
                raise AssertionError(f"unexpected {name} element")

    return check


def run_semantic_tests(root: Path, program: Path) -> int:
    """Run the inspect semantic tests."""

    return run_semantic_cases(required_file(program), semantic_cases(root))
