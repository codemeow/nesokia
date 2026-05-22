from __future__ import annotations

import subprocess

from dataclasses import dataclass, field
from pathlib import Path

from tests.helpers.output import print_failed, print_indented, print_passed
from tests.helpers.runner import run


def _empty_str_list() -> list[str]:
    """Return a new empty string list for dataclass defaults."""

    return []


@dataclass(frozen=True)
class CliCase:
    """One CLI argument parsing test command setup."""

    name: str
    args: list[str]
    expected_code: int = 0
    stdout_required: bool = False
    stderr_required: bool = False
    stdout_forbidden: bool = False
    stderr_forbidden: bool = False
    stdout_contains: list[str] = field(default_factory=_empty_str_list)
    stderr_contains: list[str] = field(default_factory=_empty_str_list)
    stdout_not_contains: list[str] = field(default_factory=_empty_str_list)
    stderr_not_contains: list[str] = field(default_factory=_empty_str_list)
    section: str | None = None


def print_process_output(proc: subprocess.CompletedProcess[str]) -> None:
    """Print captured process output for a failed CLI test."""

    if proc.stdout:
        print_indented("stdout:")
        print_indented(proc.stdout.rstrip())
    if proc.stderr:
        print_indented("stderr:")
        print_indented(proc.stderr.rstrip())


def _check_required_output(
    case: CliCase,
    proc: subprocess.CompletedProcess[str]
) -> bool:
    """Check required stdout and stderr constraints."""

    if case.stdout_required and not proc.stdout.strip():
        print_failed(case.name, "stdout is empty")
        print_process_output(proc)
        return False

    if case.stderr_required and not proc.stderr.strip():
        print_failed(case.name, "stderr is empty")
        print_process_output(proc)
        return False

    for value in case.stdout_contains:
        if value not in proc.stdout:
            print_failed(case.name, f"stdout does not contain {value!r}")
            print_process_output(proc)
            return False

    for value in case.stderr_contains:
        if value not in proc.stderr:
            print_failed(case.name, f"stderr does not contain {value!r}")
            print_process_output(proc)
            return False

    return True


def _check_forbidden_output(
    case: CliCase,
    proc: subprocess.CompletedProcess[str]
) -> bool:
    """Check forbidden stdout and stderr constraints."""

    if case.stdout_forbidden and proc.stdout.strip():
        print_failed(case.name, "stdout is not empty")
        print_process_output(proc)
        return False

    if case.stderr_forbidden and proc.stderr.strip():
        print_failed(case.name, "stderr is not empty")
        print_process_output(proc)
        return False

    for value in case.stdout_not_contains:
        if value in proc.stdout:
            print_failed(case.name, f"stdout contains {value!r}")
            print_process_output(proc)
            return False

    for value in case.stderr_not_contains:
        if value in proc.stderr:
            print_failed(case.name, f"stderr contains {value!r}")
            print_process_output(proc)
            return False

    return True


def run_cli_case(program: Path, case: CliCase) -> bool:
    """Run one CLI argument parsing test case."""

    cmd = [
        str(program),
        *case.args
    ]

    try:
        proc = run(
            cmd,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )
    except Exception as e:
        print_failed(case.name, str(e))
        return False

    if proc.returncode != case.expected_code:
        print_failed(
            case.name,
            f"exit code {proc.returncode}, expected {case.expected_code}"
        )
        print_process_output(proc)
        return False

    if not _check_required_output(case, proc):
        return False

    if not _check_forbidden_output(case, proc):
        return False

    print_passed(case.name)
    return True


def run_cli_cases(program: Path, cases: list[CliCase]) -> int:
    """Run CLI argument parsing test cases and return a process status."""

    passed = 0
    section = None

    for case in cases:
        if case.section != section:
            if section is not None:
                print()
            section = case.section
            if section is not None:
                print(f"# {section}")

        if run_cli_case(program, case):
            passed += 1

    total = len(cases)
    print()
    print(f" - Passed: {passed} out of {total}")

    return 0 if passed == total else 1
