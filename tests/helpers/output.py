import textwrap

from colorama import Fore, Style, init
from typing import Optional


init(autoreset=True)


def print_passed(message: str) -> None:
    """Print a passed test line."""

    print(f"[{Fore.GREEN}OK{Style.RESET_ALL}]    {message}")


def print_failed(message: str, reason: Optional[str] = None) -> None:
    """Print a failed test line."""

    if reason is None:
        print(f"[{Fore.RED}ERR{Style.RESET_ALL}]   {message}")
    else:
        print(f"[{Fore.RED}ERR{Style.RESET_ALL}]   {message}: {reason}")


def print_indented(output: str) -> None:
    """Print output with test indentation."""

    print(f"{textwrap.indent(output, '    ')}")
