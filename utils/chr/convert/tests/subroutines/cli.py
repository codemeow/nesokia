from pathlib import Path
from typing import Any, List

from tests.helpers.cli import CliCase, run_cli_cases

from subroutines.common import fixture, required_file, template_2c02


UNKNOWN_VALUE = "unknown"


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


def duplicate_cases(root: Path) -> List[CliCase]:
    """Build duplicate value option cases."""

    full = str(template_2c02(root))
    colors = str(fixture(root, "colors.png"))
    palettes = str(fixture(root, "palettes0.png"))
    table0 = str(fixture(root, "table0.png"))
    table1 = str(fixture(root, "table1.png"))

    cases = [
        ("mode", ["-M", "t2c", "-M", "c2t"], "mode"),
        ("back-address", ["-A", "0000", "-A", "1000"], "back-address"),
        ("input", ["-i", full, "-i", full], "input"),
        ("input-ppu-colors", ["-c", colors, "-c", colors], "input-ppu-colors"),
        ("input-palettes", ["-p", palettes, "-p", palettes], "input-palettes"),
        ("input-palette-back", ["-b", "a", "-b", "b"], "input-palette-back"),
        (
            "input-palette-sprites",
            ["-s", "a", "-s", "b"],
            "input-palette-sprites"
        ),
        (
            "input-pattern-tables",
            ["-t", "a", "-t", "b"],
            "input-pattern-tables"
        ),
        (
            "input-left-table",
            ["-l", table0, "-l", table0],
            "input-left-table"
        ),
        (
            "input-right-table",
            ["-r", table1, "-r", table1],
            "input-right-table"
        ),
        (
            "explicit-palettes",
            ["-e", "l00=0", "-e", "l01=1"],
            "explicit-palettes"
        ),
        ("output", ["-O", "a", "-O", "b"], "output"),
        ("output-ppu-colors", ["-C", "a", "-C", "b"], "output-ppu-colors"),
        ("output-palettes", ["-P", "a", "-P", "b"], "output-palettes"),
        (
            "output-palette-back",
            ["-B", "a", "-B", "b"],
            "output-palette-back"
        ),
        (
            "output-palette-sprites",
            ["-S", "a", "-S", "b"],
            "output-palette-sprites"
        ),
        (
            "output-pattern-tables",
            ["-T", "a", "-T", "b"],
            "output-pattern-tables"
        ),
        ("output-left-table", ["-L", "a", "-L", "b"], "output-left-table"),
        ("output-right-table", ["-R", "a", "-R", "b"], "output-right-table")
    ]

    return [
        section_case(
            "Duplicates",
            name=f"repeated {name}",
            args=args,
            expected_code=1,
            stderr_contains=["only one", option],
            stdout_forbidden=True
        )
        for name, args, option in cases
    ]


def cli_cases(root: Path) -> List[CliCase]:
    """Build the chr-convert CLI argument parsing test cases list."""

    full = str(template_2c02(root))

    return [
        section_case(
            "Basic",
            name="unknown short option",
            args=["-z"],
            expected_code=1,
            stderr_contains=["Unknown option"],
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
            args=["-i"],
            expected_code=1,
            stderr_contains=["Option -i requires an argument"],
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="missing long option argument",
            args=["--input"],
            expected_code=1,
            stderr_contains=["Option --input requires an argument"],
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="unexpected positional argument",
            args=["extra"],
            expected_code=1,
            stderr_contains=["Unexpected positional argument", "extra"],
            stdout_forbidden=True
        ),
        section_case(
            "Mode",
            name="invalid mode",
            args=["-M", UNKNOWN_VALUE],
            expected_code=1,
            stderr_contains=["Unknown mode option", UNKNOWN_VALUE],
            stdout_forbidden=True
        ),
        section_case(
            "Mode",
            name="invalid back-address",
            args=["-A", UNKNOWN_VALUE],
            expected_code=1,
            stderr_contains=["Unknown back-address option", UNKNOWN_VALUE],
            stdout_forbidden=True
        ),
        section_case(
            "Explicit",
            name="invalid explicit-palettes",
            args=["-e", UNKNOWN_VALUE],
            expected_code=1,
            stderr_contains=["cannot parse list string", UNKNOWN_VALUE],
            stdout_forbidden=True
        ),
        section_case(
            "Files",
            name="positional after valid options",
            args=["-q", "-i", full, "-T", "out.pats", "extra"],
            expected_code=1,
            stderr_contains=["Unexpected positional argument", "extra"],
            stdout_forbidden=True
        ),
        *duplicate_cases(root)
    ]


def run_cli_tests(root: Path, program: Path) -> int:
    """Run the chr-convert CLI argument parsing tests."""

    return run_cli_cases(required_file(program), cli_cases(root))
