#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import subprocess
from pathlib import Path

DIR_CASES            = "cases"
DIR_HELPERS          = "subroutines"

CONFIG_FILE_XML      = "nes20db-slice.xml"
CONFIG_FILE_NSK      = "nsk_header_config.inc"
CONFIG_FILE_CONSTS   = "nsk_header_consts.inc"

HELPER_CONFIG_CREATE = "nsk_config_create.py"

def main() -> None:
    print("# Creating configuration files")

    here = Path(__file__).resolve().parent
    cases_dir = here / DIR_CASES
    generator = here / DIR_HELPERS / HELPER_CONFIG_CREATE
    consts_path = (here.parent / CONFIG_FILE_CONSTS).resolve()

    if not generator.is_file():
        sys.exit(f"generator not found: {generator}")
    if not consts_path.is_file():
        sys.exit(f"consts not found: {consts_path}")
    if not cases_dir.is_dir():
        sys.exit(f"cases dir not found: {cases_dir}")

    for sub in sorted(cases_dir.iterdir()):
        if not sub.is_dir():
            continue

        xml_path = sub / CONFIG_FILE_XML
        if not xml_path.is_file():
            raise RuntimeError(f"{sub} does not contain {CONFIG_FILE_XML}")

        out_path = sub / CONFIG_FILE_NSK
        cmd = [sys.executable, str(generator), "-x", str(xml_path), "-c", str(consts_path)]

        try:
            with out_path.open("w", encoding="utf-8") as fout:
                proc = subprocess.run(cmd, stdout=fout, stderr=subprocess.PIPE, text=True)
            if proc.returncode != 0:
                sys.stderr.write(f"[ERROR] {sub.name}: exit {proc.returncode}\n{proc.stderr}\n")
            else:
                print(f"[OK]    {sub.name} -> {out_path.name}")

        except Exception as e:
            sys.stderr.write(f"[EXC]   {sub.name}: {e}\n")

if __name__ == "__main__":
    main()
