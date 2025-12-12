#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import re

from typing import Dict, Optional, Tuple
from xml.etree.ElementTree import Element
import xml.etree.ElementTree as ET

##
## \brief  Parses the scope tree from the provided file
##
## \param  consts_path  The consts file path
## \return Scopes tree
##
def parseconst_scopes(
    consts_path : str
) -> Dict[str, Dict[int, str]]:
    with open(consts_path, "r", encoding="utf-8", errors="ignore") as f:
        lines = f.readlines()

    re_scope    = re.compile(r'^\s*\.scope\s+([A-Za-z_]\w*)\s*$')
    re_endscope = re.compile(r'^\s*\.endscope\b')
    re_kv       = re.compile(r'^\s*([A-Za-z_]\w*)\s*=\s*([0-9]+)\b')

    # Stack of the recursive scope lists (`["NSK", "DEVICES", "KEDA"]`)
    stack      = []
    scope_maps = {}

    def fold_key_and_name(
        name: str
    ) -> Tuple[Optional[str], Optional[str]]:
        if len(stack) < 2 or stack[0] != "NSK":
            return None, None
        first_level = stack[1]
        rest_levels = stack[2:]
        scoped_key = f"NSK::{first_level}"
        if rest_levels:
            folded_name = "::".join(rest_levels + [name])
        else:
            folded_name = name
        return scoped_key, folded_name

    for raw in lines:
        line = raw.split(';', 1)[0].rstrip()
        if not line:
            continue

        m = re_scope.match(line)
        if m:
            stack.append(m.group(1))
            continue

        if re_endscope.match(line):
            if stack:
                stack.pop()
            continue

        kv = re_kv.match(line)
        if kv:
            scoped_key, folded_name = fold_key_and_name(kv.group(1))
            if scoped_key is None:
                continue
            val = int(kv.group(2))
            mp = scope_maps.setdefault(scoped_key, {})
            if val not in mp:
                mp[val] = folded_name

    return scope_maps

##
## \brief  Extracts the constant name by the provided integer value from
## the previously parsed scopes
##
## \param  scope_name  The scope name
## \param  value       The value
## \param  scope_maps  The scope maps
## \return String constant name
##
def scope_value(
    scope_name :str,
    value : int,
    scope_maps : Dict[str, Dict[int, str]]
) -> str:
    mp = scope_maps.get(scope_name)
    if not mp:
        raise RuntimeError(f"{scope_name} not found in the parsed file")

    name = mp.get(value)
    if name is None:
        raise RuntimeError(f"{value} not found in the parsed {scope_name}")

    return f"{scope_name}::{name}"

##
## \brief  Converts the char representation of the mirroring into bits value
##
## \param  mapper_id   The mapper identifier
## \param  value_raw   The value raw value ('H'/'V'/'0'/'1'/'4')
## \param  field_name  The requested field name
## \return Converted bit
##
def mirroring_convert(
    mapper_id: int,
    value_raw: str,
    field_name: str
) -> int:
    XML_MIRROR_RULES = {
        30: {
            "H": {"alternative": 0, "hardwired": 0},
            "V": {"alternative": 0, "hardwired": 1},
            "1": {"alternative": 1, "hardwired": 0},
            "4": {"alternative": 1, "hardwired": 1},
        },
        218: {
            "H": {"alternative": 0, "hardwired": 0},
            "V": {"alternative": 0, "hardwired": 1},
            "0": {"alternative": 1, "hardwired": 0},
            "1": {"alternative": 1, "hardwired": 1},
        },
        "other": {
            "H": {"alternative": 0, "hardwired": 0},
            "V": {"alternative": 0, "hardwired": 1},
            "4": {"alternative": 1, "hardwired": 0},
        },
    }
    rules = XML_MIRROR_RULES.get(mapper_id, XML_MIRROR_RULES["other"])
    entry = rules.get(value_raw)
    if entry is None:
        raise RuntimeError(f"mirroring rule not found: mapper={mapper_id}, raw='{value_raw}'")
    if field_name not in ("alternative", "hardwired"):
        raise RuntimeError(f"invalid field_name: '{field_name}'")
    return entry[field_name]

##
## \brief  Common accessor of the XML value
##
## Sample XML format
## - Some fields might be missing
## ~~~
## <?xml version="1.0" encoding="UTF-8"?>
## <nes20db date="2021-12-25">
##     <!-- skipped -->
##     <game>
##         <rom size="393216" crc32="..." sha1="..." />
##
##         <prgrom size="262144" crc32="..." sha1="..." sum16="68A2" />
##         <chrrom size="131072" crc32="..." sha1="..." sum16="4A96" />
##         <prgram size="8192" />
##         <chrram size="131072" />
##         <prgnvram size="8192" />
##         <chrnvram size="32768" />
##         <miscrom size="32768" crc32="..." sha1="..." number="1"/>
##
##         <pcb mapper="4" submapper="0" mirroring="H" battery="0" />
##         <trainer size="512" crc32="..." sha1="..." />
##         <console type="0" region="0" />
##         <vs hardware="5" ppu="2" />
##         <expansion type="1" />
##     </game>
##     <!-- skipped -->
## </nes20db>
## ~~~
##
## \param  xml_root        The xml root
## \param  tag_name        The tag name
## \param  attribute_name  The attribute name
## \param  required        Is this field required to exist?
## \return The extracted value or None
##
def parserxml_common(
    xml_root: Element,
    tag_name: str,
    attribute_name: str,
    required: bool
) -> Optional[str]:
    game = xml_root.find("game")
    if game is None:
        if required:
            raise RuntimeError("game not found")
        return None

    el = game.find(tag_name)
    if el is None:
        if required:
            raise RuntimeError(f"game/{tag_name} not found")
        return None

    val = el.get(attribute_name)
    if val is None:
        if required:
            raise RuntimeError(f"game/{tag_name}@{attribute_name} not found")
        return None

    return val

##
## \brief  Safely converts input value to integer
##
## \param  val  The value or None
## \return The integer or None
##
def parserxml_safe_int(val: Optional[str]) -> Optional[int]:
    return int(val) if val is not None else None

##
## \brief  Extracts the console type
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_console_type(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "console", "type", required=True)
    return parserxml_safe_int(val)

##
## \brief  Extracts the console region
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_region(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "console", "region", required=True)
    return parserxml_safe_int(val)

##
## \brief  Extracts the mapper id
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_mapper_id(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "pcb", "mapper", required=True)
    return parserxml_safe_int(val)

##
## \brief  Extracts the submapper id
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_submapper_id(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "pcb", "submapper", required=True)
    return parserxml_safe_int(val)

##
## \brief  Extracts the nametables layout
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_nametable_hardwired(
    xml_root: Element
) -> Optional[int]:
    mapper_id = parserxml_common(xml_root, "pcb", "mapper", required=True)
    mirroring_raw = parserxml_common(xml_root, "pcb", "mirroring", required=True)
    return mirroring_convert(int(mapper_id), mirroring_raw, "hardwired")

##
## \brief  Extracts the alternative nametables flag
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_nametable_alternative(
    xml_root: Element
) -> Optional[int]:
    mapper_id = parserxml_common(xml_root, "pcb", "mapper", required=True)
    mirroring_raw = parserxml_common(xml_root, "pcb", "mirroring", required=True)
    return mirroring_convert(int(mapper_id), mirroring_raw, "alternative")

##
## \brief  Extracts the battery flag
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_battery(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "pcb", "battery", required=True)
    return parserxml_safe_int(val)

##
## \brief  Extracts the trainer flag
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_trainer(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "trainer", "size", required=False)
    return 1 if val is not None else 0

##
## \brief  Extracts the PRG ROM size
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_prgrom_size(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "prgrom", "size", required=False)
    return parserxml_safe_int(val)

##
## \brief  Extracts the CHR ROM size
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_chrrom_size(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "chrrom", "size", required=False)
    return parserxml_safe_int(val)

##
## \brief  Extracts the PRG RAM size
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_prgram_size(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "prgram", "size", required=False)
    return parserxml_safe_int(val)

##
## \brief  Extracts the CHR RAM size
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_chrram_size(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "chrram", "size", required=False)
    return parserxml_safe_int(val)

##
## \brief  Extracts the PRG NVRAM size
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_prgnvram_size(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "prgnvram", "size", required=False)
    return parserxml_safe_int(val)

##
## \brief  Extracts the CHR NVRAM size
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_chrnvram_size(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "chrnvram", "size", required=False)
    return parserxml_safe_int(val)

##
## \brief  Extracts the misc ROMs count
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_roms_misc(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "miscrom", "size", required=False)
    return parserxml_safe_int(val)

##
## \brief  Extracts the Vs. PPU type
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_vs_ppu(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "vs", "ppu", required=False)
    return parserxml_safe_int(val)

##
## \brief  Extracts the Vs. Hardware type
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_vs_hardware(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "vs", "hardware", required=False)
    return parserxml_safe_int(val)

##
## \brief  Extracts the default expansion device
##
## \param  xml_root  The xml root
## \return Extracted value
##
def parserxml_expansion_device(
    xml_root: Element
) -> Optional[int]:
    val = parserxml_common(xml_root, "expansion", "type", required=False)
    return parserxml_safe_int(val)

##
## Header constants mappings
##
CONFIG_DICT = {
    "NSK_HEADER_CONSOLE_TYPE": {
        "func": parserxml_console_type,
        "scope": "NSK::CONSOLE"
    },
    "NSK_HEADER_REGION": {
        "func": parserxml_region,
        "scope": "NSK::REGION"
    },
    "NSK_HEADER_MAPPER_ID": {
        "func": parserxml_mapper_id,
        "scope": None
    },
    "NSK_HEADER_SUBMAPPER_ID": {
        "func": parserxml_submapper_id,
        "scope": None
    },
    "NSK_HEADER_NAMETABLE_HARDWIRED": {
        "func": parserxml_nametable_hardwired,
        "scope": "NSK::NM_LAYOUT"
    },
    "NSK_HEADER_NAMETABLE_ALTERNATIVE": {
        "func": parserxml_nametable_alternative,
        "scope": "NSK::NM_ALTERNATIVE"
    },
    "NSK_HEADER_BATTERY": {
        "func": parserxml_battery,
        "scope": "NSK::BATTERY"
    },
    "NSK_HEADER_TRAINER": {
        "func": parserxml_trainer,
        "scope": "NSK::TRAINER"
    },
    "NSK_HEADER_PRGROM_SIZE": {
        "func": parserxml_prgrom_size,
        "scope": None
    },
    "NSK_HEADER_CHRROM_SIZE": {
        "func": parserxml_chrrom_size,
        "scope": None
    },
    "NSK_HEADER_PRGRAM_SIZE": {
        "func": parserxml_prgram_size,
        "scope": None
    },
    "NSK_HEADER_CHRRAM_SIZE": {
        "func": parserxml_chrram_size,
        "scope": None
    },
    "NSK_HEADER_PRGNVRAM_SIZE": {
        "func": parserxml_prgnvram_size,
        "scope": None
    },
    "NSK_HEADER_CHRNVRAM_SIZE": {
        "func": parserxml_chrnvram_size,
        "scope": None
    },
    "NSK_HEADER_ROMS_MISC": {
        "func": parserxml_roms_misc,
        "scope": None
    },
    "NSK_HEADER_VS_PPU": {
        "func": parserxml_vs_ppu,
        "scope": "NSK::VSPPU"
    },
    "NSK_HEADER_VS_HARDWARE": {
        "func": parserxml_vs_hardware,
        "scope": "NSK::VSHARDWARE"
    },
    "NSK_HEADER_EXPANSION_DEVICE": {
        "func": parserxml_expansion_device,
        "scope": "NSK::DEVICES"
    },
}

##
## \brief  Prints the file header
##
def print_header() -> None:
    header = """\
; @file nsk_header_config.inc
; @brief Configuration for NES 2.0 header generation.
;
; Provides sample configuration with additional comments
;
; Part of the Nesokia project — MIT License.
"""
    print(header)

##
## \brief  Main function
##
def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("-x", "--xml", required=True, help="Path to input XML slice file")
    ap.add_argument("-c", "--consts", required=True, help="Path to nsk_header_consts.inc to parse scopes")
    args = ap.parse_args()

    xml_root = ET.parse(args.xml).getroot()

    scope_maps = parseconst_scopes(args.consts)

    print_header()

    for key, payload in CONFIG_DICT.items():
        cb = payload["func"]
        try:
            value = cb(xml_root)
            if value is None:
                # No such field in the XML
                # Does not matter then, will be treated as default value
                pass
            else:
                prefix = ""

                if value == 0:
                    # Purely cosmetic, zero values are set by default
                    prefix = "; "

                if payload["scope"] is None:
                   # Raw value, such as sizes
                   print(f"{prefix}::{key} = {value}")
                else:
                   # Converted value, named constants
                   name = scope_value(payload["scope"], value, scope_maps)
                   print(f"{prefix}::{key} = {name}")

        except Exception as e:
            raise RuntimeError(f"Failed to get XML value within {key}: {e}") from e

if __name__ == "__main__":
    main()
