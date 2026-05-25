from pathlib import Path
from typing import Any, List

from tests.helpers.cli import CliCase, run_cli_cases

from subroutines.common import required_file, template_2c02


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


def cli_cases(root: Path) -> List[CliCase]:
    """Build the chr-colors CLI argument parsing test cases list."""

    input_file = str(template_2c02(root))
    output_file = "out.gpl"

    return [
        section_case(
            "Basic",
            name="no arguments",
            args=[],
            expected_code=1,
            stderr_contains=["No input file provided"],
            stdout_forbidden=True
        ),
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
            stderr_contains=["requires an argument"],
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="missing long option argument",
            args=["--input"],
            expected_code=1,
            stderr_contains=["requires an argument"],
            stdout_forbidden=True
        ),
        section_case(
            "Input",
            name="repeated input option",
            args=[
                "-q",
                "-i",
                input_file,
                "-i",
                input_file,
                "-o",
                output_file
            ],
            expected_code=1,
            stderr_contains=["only one", "input"],
            stdout_forbidden=True
        ),
        section_case(
            "Input",
            name="invalid input format option",
            args=[
                "-q",
                "-i",
                input_file,
                "-I",
                UNKNOWN_VALUE,
                "-o",
                output_file
            ],
            expected_code=1,
            stderr_contains=["Unknown input format", UNKNOWN_VALUE],
            stdout_forbidden=True
        ),
        section_case(
            "Input",
            name="repeated input format option",
            args=[
                "-q",
                "-i",
                input_file,
                "-I",
                "pal",
                "-I",
                "pal",
                "-o",
                output_file
            ],
            expected_code=1,
            stderr_contains=["only one", "input-format"],
            stdout_forbidden=True
        ),
        section_case(
            "Output",
            name="repeated output option",
            args=[
                "-q",
                "-i",
                input_file,
                "-o",
                "out1.gpl",
                "-o",
                "out2.gpl"
            ],
            expected_code=1,
            stderr_contains=["only one", "output"],
            stdout_forbidden=True
        ),
        section_case(
            "Output",
            name="invalid output format option",
            args=[
                "-q",
                "-i",
                input_file,
                "-o",
                output_file,
                "-O",
                UNKNOWN_VALUE
            ],
            expected_code=1,
            stderr_contains=["Unknown output format", UNKNOWN_VALUE],
            stdout_forbidden=True
        ),
        section_case(
            "Output",
            name="repeated output format option",
            args=[
                "-q",
                "-i",
                input_file,
                "-o",
                output_file,
                "-O",
                "gpl",
                "-O",
                "gpl"
            ],
            expected_code=1,
            stderr_contains=["only one", "output-format"],
            stdout_forbidden=True
        ),
        section_case(
            "Files",
            name="positional argument",
            args=[
                "-q",
                "-i",
                input_file,
                "-o",
                output_file,
                "extra"
            ],
            expected_code=1,
            stderr_contains=["Unexpected positional argument", "extra"],
            stdout_forbidden=True
        )
    ]


def run_cli_tests(root: Path, program: Path) -> int:
    """Run the chr-colors CLI argument parsing tests."""

    return run_cli_cases(required_file(program), cli_cases(root))
