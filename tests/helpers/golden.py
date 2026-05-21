from __future__ import annotations

import difflib
import subprocess

from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional

from tests.helpers.output import print_failed, print_indented, print_passed


@dataclass(frozen=True)
class GoldenCase:
    """One golden output comparison test command setup."""

    name: str
    args: List[str]
    expected: Path
    cwd: Path
    expected_code: int = 0
    stderr_forbidden: bool = True
    section: Optional[str] = None


def print_process_output(proc: subprocess.CompletedProcess[str]) -> None:
    """Print captured process output for a failed golden test."""

    if proc.stdout:
        print_indented("stdout:")
        print_indented(proc.stdout.rstrip())
    if proc.stderr:
        print_indented("stderr:")
        print_indented(proc.stderr.rstrip())


def print_diff(expected: str, actual: str) -> None:
    """Print a unified diff between expected and actual output."""

    diff = difflib.unified_diff(
        expected.splitlines(keepends=True),
        actual.splitlines(keepends=True),
        fromfile="expected",
        tofile="actual"
    )
    print_indented("".join(diff).rstrip())


def run_golden_case(program: Path, case: GoldenCase) -> bool:
    """Run one golden output comparison test case."""

    cmd = [
        str(program),
        *case.args
    ]

    try:
        expected = case.expected.read_text(encoding="utf-8")
        proc = subprocess.run(
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

    if case.stderr_forbidden and proc.stderr.strip():
        print_failed(case.name, "stderr is not empty")
        print_process_output(proc)
        return False

    if proc.stdout != expected:
        print_failed(case.name, "stdout differs from golden output")
        print_diff(expected, proc.stdout)
        return False

    print_passed(case.name)
    return True


def run_golden_cases(program: Path, cases: List[GoldenCase]) -> int:
    """Run golden output comparison test cases and return a process status."""

    passed = 0
    section = None

    for case in cases:
        if case.section != section:
            if section is not None:
                print()
            section = case.section
            if section is not None:
                print(f"# {section}")

        if run_golden_case(program, case):
            passed += 1

    total = len(cases)
    print()
    print(f" - Passed: {passed} out of {total}")

    return 0 if passed == total else 1
