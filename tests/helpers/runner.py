from __future__ import annotations

import os
import subprocess
from pathlib import Path
from typing import Literal, Sequence, Union


Command = Sequence[Union[str, Path]]

VALGRIND_OPTIONS = [
    "--tool=memcheck",
    "--leak-check=full",
    "--show-leak-kinds=all",
    "--errors-for-leak-kinds=all",
    "--track-origins=yes",
    "--num-callers=50",
    "--error-exitcode=125",
    "--quiet",
    "--log-fd=2"
]


def _valgrind_enabled() -> bool:
    """Return true if tests should run nesokia binaries under Valgrind."""

    return os.environ.get("NESOKIA_TEST_VALGRIND") == "1"


def _is_nesokia_binary(command: Command) -> bool:
    """Return true if the command starts a nesokia binary."""

    if not command:
        return False

    return Path(command[0]).name.startswith("nesokia-")


def run(
    command: Command,
    *,
    cwd: str | Path | None = None,
    text: Literal[True] = True,
    stdout: int | None = None,
    stderr: int | None = None
) -> subprocess.CompletedProcess[str]:
    """Run a command, optionally wrapping nesokia binaries with Valgrind."""

    normalized = [str(part) for part in command]
    cwd_arg = None if cwd is None else str(cwd)
    if not _valgrind_enabled() or not _is_nesokia_binary(normalized):
        return subprocess.run(
            normalized,
            cwd=cwd_arg,
            text=text,
            stdout=stdout,
            stderr=stderr
        )

    wrapped = [
        "valgrind",
        *VALGRIND_OPTIONS,
        *normalized
    ]
    return subprocess.run(
        wrapped,
        cwd=cwd_arg,
        text=text,
        stdout=stdout,
        stderr=stderr
    )
