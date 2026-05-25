from pathlib import Path
from typing import Any, List, Union

from tests.helpers.cli import CliCase, run_cli_cases
from tests.helpers.nes_cases import nes_cases


UNKNOWN_VALUE = "unknown"

BAD_FILTERS = [
    "mapper",
    "mapper==0",
    "mapper=abc",
    "mapper=0&"
]

BAD_KEYS = [
    "mapper&",
    "mapper&&prg_rom"
]


def section_case(
    section: str,
    name: str,
    args: List[str],
    **kwargs: Any
) -> CliCase:
    """Build a CLI case for an output section."""

    return CliCase(
        name=name,
        args=args,
        section=section,
        **kwargs
    )


def required_file(path: Union[str, Path]) -> Path:
    """Resolve a file path and require it to exist."""

    filename = Path(path).resolve()
    if not filename.is_file():
        raise RuntimeError(f"File {filename} not found")
    return filename


def cli_cases(root: Path) -> List[CliCase]:
    """Build the CLI argument parsing test cases list."""

    shared_cases = nes_cases(root)
    first_rom = str(shared_cases[0].reference)
    second_rom = str(shared_cases[1].reference)

    return [
        section_case(
            "Basic",
            name="no arguments",
            args=[],
            expected_code=1,
            stderr_contains=["no input paths provided"],
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="unknown short option",
            args=["-z"],
            expected_code=1,
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="unknown long option",
            args=["--unknown"],
            expected_code=1,
            stderr_contains=["Unknown option"],
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="missing short option argument",
            args=["-o"],
            expected_code=1,
            stderr_contains=["requires an argument"],
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="missing long option argument",
            args=["--output"],
            expected_code=1,
            stderr_contains=["requires an argument"],
            stdout_forbidden=True
        ),
        section_case(
            "Output",
            name="repeated output option uses last value",
            args=["-o", "json", "-o", "table", first_rom],
            stdout_contains=["| Filename", first_rom],
            stdout_not_contains=["\"filename\""],
            stderr_forbidden=True
        ),
        section_case(
            "Output",
            name="invalid output value",
            args=["-o", UNKNOWN_VALUE, first_rom],
            expected_code=1,
            stderr_contains=["invalid value", UNKNOWN_VALUE],
            stdout_forbidden=True
        ),
        section_case(
            "Files",
            name="multiple rom files",
            args=["-o", "json", first_rom, second_rom],
            stdout_contains=[
                "\"filename\"",
                first_rom,
                second_rom
            ],
            stderr_forbidden=True
        ),
        section_case(
            "Combinations",
            name="keys with table output",
            args=["-k", "mapper", "-o", "table", first_rom],
            stdout_contains=["| Filename", "Mapper ID"],
            stdout_not_contains=["PRG ROM"],
            stderr_forbidden=True
        ),
        section_case(
            "Combinations",
            name="filter with keys and table output",
            args=[
                "-f", "mapper>=0",
                "-k", "mapper",
                "-o", "table",
                first_rom
            ],
            stdout_contains=["| Filename", "Mapper ID"],
            stdout_not_contains=["PRG ROM"],
            stderr_forbidden=True
        ),
        section_case(
            "Combinations",
            name="quiet keeps invalid output error",
            args=["-q", "-o", UNKNOWN_VALUE, first_rom],
            expected_code=1,
            stderr_contains=["invalid value", UNKNOWN_VALUE],
            stdout_forbidden=True
        ),
        *filter_syntax_cases(first_rom),
        *keys_syntax_cases(first_rom)
    ]


def filter_syntax_cases(rom: str) -> List[CliCase]:
    """Build invalid filter syntax test cases."""

    return [
        section_case(
            "Filter Syntax",
            name=bad_filter,
            args=["-f", bad_filter, rom],
            expected_code=1,
            stderr_contains=["cannot parse list string"],
            stdout_forbidden=True
        )
        for bad_filter in BAD_FILTERS
    ]


def keys_syntax_cases(rom: str) -> List[CliCase]:
    """Build invalid keys syntax test cases."""

    return [
        section_case(
            "Keys Syntax",
            name=bad_keys,
            args=["-k", bad_keys, rom],
            expected_code=1,
            stderr_contains=["cannot parse list string"],
            stdout_forbidden=True
        )
        for bad_keys in BAD_KEYS
    ]


def run_cli_tests(root: Path, program: Path) -> int:
    """Run the inspect CLI argument parsing tests."""

    return run_cli_cases(required_file(program), cli_cases(root))
