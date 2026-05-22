from __future__ import annotations

import subprocess

from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional

from tests.helpers.output import print_failed, print_indented, print_passed
from tests.helpers.runner import run


@dataclass(frozen=True)
class SmokeCase:
    """One smoke test command setup."""

    name: str
    args: List[str]
    expected_code: int = 0
    stdout_required: bool = False
    stderr_required: bool = False
    stdout_forbidden: bool = False
    stderr_forbidden: bool = False
    section: Optional[str] = None


def print_process_output(proc: subprocess.CompletedProcess[str]) -> None:
    """Print captured process output for a failed smoke test."""

    if proc.stdout:
        print_indented("stdout:")
        print_indented(proc.stdout.rstrip())
    if proc.stderr:
        print_indented("stderr:")
        print_indented(proc.stderr.rstrip())


def run_smoke_case(program: Path, case: SmokeCase) -> bool:
    """Run one smoke test case."""

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

    if case.stdout_required and not proc.stdout.strip():
        print_failed(case.name, "stdout is empty")
        print_process_output(proc)
        return False

    if case.stderr_required and not proc.stderr.strip():
        print_failed(case.name, "stderr is empty")
        print_process_output(proc)
        return False

    if case.stdout_forbidden and proc.stdout.strip():
        print_failed(case.name, "stdout is not empty")
        print_process_output(proc)
        return False

    if case.stderr_forbidden and proc.stderr.strip():
        print_failed(case.name, "stderr is not empty")
        print_process_output(proc)
        return False

    print_passed(case.name)
    return True


def run_smoke_cases(program: Path, cases: List[SmokeCase]) -> int:
    """Run smoke test cases and return a process status."""

    passed = 0
    section = None

    for case in cases:
        if case.section != section:
            if section is not None:
                print()
            section = case.section
            if section is not None:
                print(f"# {section}")

        if run_smoke_case(program, case):
            passed += 1

    total = len(cases)
    print()
    print(f" - Passed: {passed} out of {total}")

    return 0 if passed == total else 1
