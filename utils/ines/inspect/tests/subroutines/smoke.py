from pathlib import Path
from typing import Any, List, Union

from tests.helpers.nes_cases import nes_cases
from tests.helpers.smoke import SmokeCase, run_smoke_cases


UNKNOWN_VALUE = "unknown"

OUTPUT_FORMATS = [
    "tree",
    "table",
    "json",
    "xml",
    "yaml",
    "md",
    "csv"
]

FILTERS = [
    "mapper=0",
    "mapper!=9999",
    "mapper<>9999",
    "mapper<4096",
    "prg_rom>0",
    "prg_rom<=1073741824",
    "mapper>=0",
    "mapper>=0&prg_rom>0",
    "mapper<4096&prg_rom<=1073741824",
    "mapper!=9999&prg_rom>0&chr_rom>=0"
]

KEY_SETS = [
    "mapper",
    "mapper&prg_rom",
    "mapper&prg_rom&chr_rom&region"
]


def section_case(
    section: str,
    name: str,
    args: List[str],
    **kwargs: Any
) -> SmokeCase:
    """Build a smoke case for an output section."""

    return SmokeCase(
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


def smoke_cases(root: Path) -> List[SmokeCase]:
    """Build the smoke test cases list."""

    shared_cases = nes_cases(root)
    recursive_root = root / "tests" / "nes"

    cases = [
        section_case(
            "Basic",
            name="help short",
            args=["-h"],
            stdout_required=True
        ),
        section_case(
            "Basic",
            name="help long",
            args=["--help"],
            stdout_required=True
        ),
        section_case(
            "Basic",
            name="version short",
            args=["-v"],
            stdout_required=True
        ),
        section_case(
            "Basic",
            name="version long",
            args=["--version"],
            stdout_required=True
        ),
        section_case(
            "Basic",
            name="output short missing argument",
            args=["-o"],
            expected_code=1,
            stderr_required=True,
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="output long missing argument",
            args=["--output"],
            expected_code=1,
            stderr_required=True,
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="filter short missing argument",
            args=["-f"],
            expected_code=1,
            stderr_required=True,
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="filter long missing argument",
            args=["--filter"],
            expected_code=1,
            stderr_required=True,
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="keys short missing argument",
            args=["-k"],
            expected_code=1,
            stderr_required=True,
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="keys long missing argument",
            args=["--keys"],
            expected_code=1,
            stderr_required=True,
            stdout_forbidden=True
        ),
        section_case(
            "Basic",
            name="recursive short",
            args=["-r", str(recursive_root)],
            stdout_required=True
        ),
        section_case(
            "Basic",
            name="recursive long",
            args=["--recursive", str(recursive_root)],
            stdout_required=True
        )
    ]

    for case in shared_cases:
        rom = str(case.reference)
        cases.append(section_case(
            "Inspect",
            name=case.name,
            args=[rom],
            stdout_required=True
        ))

    for output_format in OUTPUT_FORMATS:
        for name, option in [
            ("short", "-o"),
            ("long", "--output")
        ]:
            for case in shared_cases:
                rom = str(case.reference)
                cases.append(section_case(
                    f"Output {name} {output_format}",
                    name=case.name,
                    args=[option, output_format, rom],
                    stdout_required=True
                ))

    for name, option in [
        ("short", "-o"),
        ("long", "--output")
    ]:
        for case in shared_cases:
            rom = str(case.reference)
            cases.append(section_case(
                f"Output {name} invalid",
                name=case.name,
                args=[option, UNKNOWN_VALUE, rom],
                expected_code=1,
                stderr_required=True,
                stdout_forbidden=True
            ))

    for name, option in [
        ("short", "-q"),
        ("long", "--quiet")
    ]:
        for case in shared_cases:
            rom = str(case.reference)
            cases.append(section_case(
                f"Quiet {name}",
                name=case.name,
                args=[option, rom],
                stdout_forbidden=True,
                stderr_forbidden=True
            ))

    for name, args in [
        ("short", ["-q", "-o", UNKNOWN_VALUE]),
        ("long", ["--quiet", "--output", UNKNOWN_VALUE])
    ]:
        for case in shared_cases:
            rom = str(case.reference)
            cases.append(section_case(
                f"Quiet {name} keeps errors",
                name=case.name,
                args=[*args, rom],
                expected_code=1,
                stderr_required=True,
                stdout_forbidden=True
            ))

    for name, option in [
        ("short", "-l"),
        ("long", "--follow")
    ]:
        for case in shared_cases:
            rom = str(case.reference)
            cases.append(section_case(
                f"Follow {name}",
                name=case.name,
                args=[option, rom],
                stdout_required=True
            ))

    for filter_value in FILTERS:
        for name, option in [
            ("short", "-f"),
            ("long", "--filter")
        ]:
            for case in shared_cases:
                rom = str(case.reference)
                cases.append(section_case(
                    f"Filter {name} {filter_value}",
                    name=case.name,
                    args=[option, filter_value, rom]
                ))

    for name, option in [
        ("short", "-f"),
        ("long", "--filter")
    ]:
        for case in shared_cases:
            rom = str(case.reference)
            cases.append(section_case(
                f"Filter {name} invalid",
                name=case.name,
                args=[option, UNKNOWN_VALUE, rom],
                expected_code=1,
                stderr_required=True,
                stdout_forbidden=True
            ))

    for key_set in KEY_SETS:
        for name, option in [
            ("short", "-k"),
            ("long", "--keys")
        ]:
            for case in shared_cases:
                rom = str(case.reference)
                cases.append(section_case(
                    f"Keys {name} {key_set}",
                    name=case.name,
                    args=[option, key_set, rom],
                    stdout_required=True
                ))

    for name, option in [
        ("short", "-k"),
        ("long", "--keys")
    ]:
        for case in shared_cases:
            rom = str(case.reference)
            cases.append(section_case(
                f"Keys {name} invalid",
                name=case.name,
                args=[option, UNKNOWN_VALUE, rom],
                expected_code=1,
                stderr_required=True,
                stdout_forbidden=True
            ))

    return cases


def run_smoke_tests(root: Path, program: Path) -> int:
    """Run the inspect smoke tests."""

    return run_smoke_cases(required_file(program), smoke_cases(root))
