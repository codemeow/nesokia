from __future__ import annotations

import subprocess

from dataclasses import dataclass
from pathlib import Path
from typing import TYPE_CHECKING, Callable, List, Optional

from tests.helpers.output import print_failed, print_indented, print_passed
from tests.helpers.runner import run


if TYPE_CHECKING:
    SemanticProcess = subprocess.CompletedProcess[str]
else:
    SemanticProcess = subprocess.CompletedProcess

SemanticCheck = Callable[[SemanticProcess], None]


@dataclass(frozen=True)
class SemanticCase:
    """One semantic test command setup."""

    name: str
    args: List[str]
    cwd: Path
    check: SemanticCheck
    expected_code: int = 0
    section: Optional[str] = None


def print_process_output(proc: SemanticProcess) -> None:
    """Print captured process output for a failed semantic test."""

    if proc.stdout:
        print_indented("stdout:")
        print_indented(proc.stdout.rstrip())
    if proc.stderr:
        print_indented("stderr:")
        print_indented(proc.stderr.rstrip())


def run_semantic_case(program: Path, case: SemanticCase) -> bool:
    """Run one semantic test case."""

    cmd = [
        str(program),
        *case.args
    ]

    try:
        proc = run(
            cmd,
            cwd=case.cwd,
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

    try:
        case.check(proc)
    except Exception as e:
        print_failed(case.name, str(e))
        print_process_output(proc)
        return False

    print_passed(case.name)
    return True


def run_semantic_cases(program: Path, cases: List[SemanticCase]) -> int:
    """Run semantic test cases and return a process status."""

    passed = 0
    section = None

    for case in cases:
        if case.section != section:
            if section is not None:
                print()
            section = case.section
            if section is not None:
                print(f"# {section}")

        if run_semantic_case(program, case):
            passed += 1

    total = len(cases)
    print()
    print(f" - Passed: {passed} out of {total}")

    return 0 if passed == total else 1
