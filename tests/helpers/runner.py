from __future__ import annotations

import os
import subprocess
from pathlib import Path
from typing import Any, Sequence, Union


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
    **kwargs: Any
) -> subprocess.CompletedProcess[str]:
    """Run a command, optionally wrapping nesokia binaries with Valgrind."""

    normalized = [str(part) for part in command]
    if not _valgrind_enabled() or not _is_nesokia_binary(normalized):
        return subprocess.run(normalized, **kwargs)

    wrapped = [
        "valgrind",
        *VALGRIND_OPTIONS,
        *normalized
    ]
    return subprocess.run(wrapped, **kwargs)
