# nesokia-head

Display information about NES ROMs (iNES / NES 2.0 headers).

## Overview

`nesokia-head` scans one or more ROM files or directories and prints metadata extracted from their iNES or NES 2.0 headers.
It supports multiple output formats (human-readable tree, table, JSON, XML, YAML, Markdown, CSV), selective field display,
powerful filtering, and recursive directory traversal.

---

## Usage

```bash
nesokia-head [options]... [file]...
```

* `file` can be one or more ROM files or directories.

---

## Options

### Filter

`-f, --filter=value`

Use a single string to filter ROM entries. The string is a sequence of pair's
joined by `&`. Only ROMs that satisfy all specified conditions will be displayed

#### Grammar

    expr  ::= pair ('&' pair)*
    pair  ::= key op value
    op    ::= '=' | '!=' | '<>' | <' | '>' | '<=' | '>='
    key   ::= [A-Za-z0-9_]+
    value ::= uint64 (decimal)

#### Semantics

* "="  — exact equality
* "!=" — not equal
* "<>" - not equal
* "<"  — strictly less than
* "<=" — less or equal
* ">"  — strictly greater than
* ">=" — greater or equal
* No spaces; `&` separates conditions.

#### Supported keys

| key               | values¹   |    description                          |
| ----------------- | --------- | --------------------------------------- |
| isnes20           |   0..1    |  iNES or NES 2.0 header format          |
| console           |   0..12   |  Console type: NES, VS System, etc      |
| region            |   0..3    |  Console region/timing (NTSC, PAL, etc) |
| mirroring         |   0..1    |  Vertical or Horizontal mirroring       |
| alternative       |   0..1    |  Normal or alternative layout           |
| mapper            |   0..4095 |  Mapper ID                              |
| submapper         |   0..15   |  Submapper ID                           |
| prg_rom           |   uint64  |  PRG ROM size                           |
| prg_ram           |   uint64  |  PRG RAM size                           |
| prg_nvram         |   uint64  |  PRG NVRAM size                         |
| chr_rom           |   uint64  |  CHR ROM size                           |
| chr_ram           |   uint64  |  CHR RAM size                           |
| chr_nvram         |   uint64  |  CHR NVRAM size                         |
| misc_roms         |   0..3    |  Miscellaneous ROMs count               |
| battery           |   0..1    |  Battery NVRAM presence flag            |
| trainer           |   0..1    |  Trainer area presence flag             |
| device            |   0..78   |  Default expansion device               |
| vs_ppu            |   0..11   |  Vs. System PPU type                    |
| vs_hardware       |   0..6    |  Vs. System hardware type               |

¹Any valid uint64 value could be passed, but only listed values make any sense.

Examples

* `mapper=305&prg_ram>0`

  Mapper ID is exactly 305 and PRG RAM size is > 0.

* `region=1&chr_rom=0`

  PAL region/timings and no CHR ROM.

* `prg_rom>262144&battery>0`

  PRG ROM larger than 256 KiB and battery-backed NVRAM present.

* `isnes20=1&submapper>0`

  iNES 2.0 with a nonzero submapper.


### Follow symbolic links

`-l, --follow`

By default no symbolic links are followed

### Print help

`-h, --help`

Prints help and exit

### Set keys to show

`-k, --keys=value`

Sets the subset of keys to be shown. By default all keys are shown instead.

The string is a sequence of keys joined by `&`.

#### Grammar

    expr  ::= key ('&' key)*
    key   ::= [A-Za-z0-9_]+

#### Supported keys

See "filter" option description

#### Examples

* `mapper&prg_rom`

  Shows only mapper and PRG ROM fields

### Output format

`-o, --output=value`

Select the output type

Available formats:

* `tree`:

  This is default output type. Shows the ROMs information as
  detailed human-readable tree-like output

<details>
<summary>
Output example (click to expand)
</summary>

```
# Test/snake.nes
 - Format:
   - Is NES 2.0?         : No
 - Console:
   - Type                : Nintendo Entertainment System / Famicom / Dendy
   - Timings / region    : RP2C02 PPU / NTSC 21.47 MHz / North America / Japan / South Korea / Taiwan
   - Vs. PPU type        : Not applicable
   - Vs. Hardware type   : Not applicable
 - Nametables:
   - Mirroring / layout  : Horizontal (or mapper controlled)
   - Alternative layout  : No
 - Mapper:
   - Mapper ID           : 4
   - Submapper ID        : 0
 - Memory:
   - PRG ROM             : 0.50 MiB (524288 bytes)
   - CHR ROM             : 256.00 KiB (262144 bytes)
   - PRG RAM             : 8.00 KiB (8192 bytes)
   - CHR RAM             : 0 bytes
   - PRG NVRAM           : 0 bytes
   - CHR NVRAM           : 0 bytes
   - Misc. ROMs          : 0
 - Miscellaneous info:
   - Trainer area        : No
   - Non volatile memory : Yes
   - Expansion device    : Unspecified/none
```

</details>

* `table`:

  Prints the ROMs information as a table
  This output is compatible with Markdown tables format

<details>
<summary>
Output example (click to expand)
</summary>

```
| Filename            | Mirroring / layout | Mapper ID |    PRG ROM |    CHR ROM |    PRG RAM |    CHR RAM |
|---------------------|--------------------|-----------|------------|------------|------------|------------|
| snake.nes           |                  0 |         4 |     524288 |     262144 |       8192 |          0 |
| Spy Hunter (U) .nes |                  0 |         3 |      32768 |      32768 |       8192 |          0 |
```

</details>

* `json`:

  Prints the ROMs information as a JSON document
  Uses the fields naming convention from the "filter" option description.

  Additionally enriches them with the "filename" field.
  Every field (except "filename") is represented as an object, containing
  "raw" and "display" values.

<details>
<summary>
Output example (click to expand)
</summary>

```json
[
  {
    "filename" : "Super C (U) .nes",
    "mapper" : {
      "mapper" : {
        "raw" : 4,
        "display" : "4"
      },
    },
    "memory" : {
      "prg_rom" : {
        "raw" : 131072,
        "display" : "128.00 KiB (131072 bytes)"
      },
    }
  },
  {
    "filename" : "Trog (U) .nes",
    "mapper" : {
      "mapper" : {
        "raw" : 2,
        "display" : "2"
      },
    },
    "memory" : {
      "prg_rom" : {
        "raw" : 131072,
        "display" : "128.00 KiB (131072 bytes)"
      },
    }
  }
]
```

</details>

* `xml`:

  Prints the ROMs information as an XML document
  This format uses the notation suggested by NewRisingSun.
  See https://forums.nesdev.org/viewtopic.php?f=3&t=19940&p=248796
  for more information.
  Quick ref: only present fields are printed, all but mirroring fields
  are numeric. The mirroring is encoded as follows:

  - Normal mappers:

    | Encoding |    byte 6  |
    |----------|------------|
    |     H    | .... .0..0 |
    |     V    | .... .0..1 |
    |     4    | .... .1..0 |

  - Mapper 030:

    | Encoding |    byte 6  |
    |----------|------------|
    |     H    | .... .0..0 |
    |     V    | .... .0..1 |
    |     1    | .... .1..0 |
    |     4    | .... .1..1 |

  - Mapper 218:

    | Encoding |    byte 6  |
    |----------|------------|
    |     H    | .... .0..0 |
    |     V    | .... .0..1 |
    |     0    | .... .1..0 |
    |     1    | .... .1..1 |

  Differences from NewRisingSun's format:
  - New field "name": ROM filename
  - New field "format": attribute "isnes20"

  Note: this output ignores the "keys" settings

<details>
<summary>
Output example (click to expand)
</summary>

```xml
<?xml version="1.0" encoding="UTF-8"?>
<scan>
  <game><!-- ./Test/test/snake.nes -->
    <name>./Test/test/snake.nes</name>
    <format isnes20="0" />
    <prgrom size="524288" />
    <chrrom size="262144" />
    <prgram size="8192" />
    <pcb mapper="4" submapper="0" mirroring="V" battery="1" />
    <console type="0" region="0" />
  </game>
  <game><!-- ./Test/test/Subor - LC V1.0 (Unl).nes -->
    <name>./Test/test/Subor - LC V1.0 (Unl).nes</name>
    <format isnes20="1" />
    <prgrom size="131072" />
    <prgram size="8192" />
    <chrram size="8192" />
    <pcb mapper="167" submapper="0" mirroring="V" battery="0" />
    <console type="0" region="3" />
    <expansion type="38" />
  </game>
</scan>

```

</details>

* `yaml`:

  Prints the ROMs information as a YAML document.
  Uses the fields naming convention from the "filter" option description.

  Additionally enriches them with the "filename" field.
  Every field (except "filename") is represented as an object, containing
  "raw" and "display" values.

<details>
<summary>
Output example (click to expand)
</summary>

```yaml
- "filename": "./Test/test/Subor - LC V1.0 (Unl).nes"
  "format":
    "isnes20":     { raw: true, display: "Yes" }
  "console":
    "console":     { raw: 0, display: "Nintendo Entertainment System / Famicom / Dendy" }
    "region":      { raw: 3, display: "NTSC/PAL hybrid / Dendy / East. Europe / Russia / China / India / Africa" }
    "vs_ppu":      { raw: null, display: "Not applicable" }
    "vs_hardware": { raw: null, display: "Not applicable" }
  "nametables":
    "mirroring":   { raw: 0, display: "Horizontal (or mapper controlled)" }
    "alternative": { raw: false, display: "No" }
  "mapper":
    "mapper":      { raw: 167, display: "167" }
    "submapper":   { raw: 0, display: "0" }
  "memory":
    "prg_rom":     { raw: 131072, display: "128.00 KiB (131072 bytes)" }
    "chr_rom":     { raw: 0, display: "0 bytes" }
    "prg_ram":     { raw: 8192, display: "8.00 KiB (8192 bytes)" }
    "chr_ram":     { raw: 8192, display: "8.00 KiB (8192 bytes)" }
    "prg_nvram":   { raw: 0, display: "0 bytes" }
    "chr_nvram":   { raw: 0, display: "0 bytes" }
    "misc_roms":   { raw: 0, display: "0" }
  "misc":
    "trainer":     { raw: false, display: "No" }
    "battery":     { raw: false, display: "No" }
    "device":      { raw: 38, display: "Subor (小霸王) keyboard" }

```

</details>

* `md`:

  Prints the ROMs information as pretty Markdown document

<details>
<summary>
Output example (click to expand)
</summary>

## `Zen - Intergalactic Ninja (U) .nes`

### Format

* **Is NES 2.0?:** No

### Console

* **Type:** Nintendo Entertainment System / Famicom / Dendy
* **Timings / region:** RP2C02 PPU / NTSC 21.47 MHz / North America / Japan / South Korea / Taiwan
* **Vs. PPU type:** Not applicable
* **Vs. Hardware type:** Not applicable

### Nametables

* **Mirroring / layout:** Horizontal (or mapper controlled)
* **Alternative layout:** No

### Mapper

* **Mapper ID:** 4
* **Submapper ID:** 0

### Memory

| Type | Size |
| ---- | ---- |
| PRG ROM | 128.00 KiB (131072 bytes) |
| CHR ROM | 128.00 KiB (131072 bytes) |
| PRG RAM | 8.00 KiB (8192 bytes) |
| CHR RAM | 0 bytes |
| PRG NVRAM | 0 bytes |
| CHR NVRAM | 0 bytes |
| Misc. ROMs | 0 |

### Miscellaneous info

* **Trainer area:** No
* **Non volatile memory:** No
* **Expansion device:** Unspecified/none

</details>

* `csv`:

  Prints the ROMs information as a comma-separated CSV document

<details>
<summary>
Result
</summary>

```csv
Filename,"Is NES 2.0?","Type","Timings / region","Vs. PPU type","Vs. Hardware type","Mirroring / layout","Alternative layout","Mapper ID","Submapper ID","PRG ROM","CHR ROM","PRG RAM","CHR RAM","PRG NVRAM","CHR NVRAM","Misc. ROMs","Trainer area","Non volatile memory","Expansion device"
"./Test/test/snake.nes",0,0,0,,,0,0,4,0,524288,262144,8192,0,0,0,0,0,1,0
"./Test/test/snake.nes",0,0,0,,,0,0,4,0,524288,262144,8192,0,0,0,0,0,1,0
"./Test/test/Spy Hunter (U) .nes",0,0,0,,,0,0,3,0,32768,32768,8192,0,0,0,0,0,0,0
"./Test/test/Subor - LC V1.0 (Unl).nes",1,0,3,,,0,0,167,0,131072,0,8192,8192,0,0,0,0,0,38
"./Test/test/tinyprg-pong.nes",1,0,0,,,0,0,0,0,4,4096,0,0,0,0,0,0,0,0
"./Test/test/測試蘭姆酒.nes",0,0,0,,,1,0,0,0,32768,8192,8192,0,0,0,0,0,0,0
```

</details>


### Quiet mode

`-q, --quiet`

Suppress any output but errors

### Recursive

`-r, --recursive`

Enable recursive directory scanning.
When specified, the program will process not only the given file(s)
but also all subdirectories.

### Show version

`-v, --version`

Print program version and exit

---

## Practical examples

Just display ROM info (`tree` output is selected by default):

```bash
nesokia-head snake.nes
```

Scan a directory, filter by PRG RAM = 8192 bytes and CHR ROM > 4000 bytes, show only mapper, PRG/CHR ROM, PRG/CHR RAM and mirroring columns:

```bash
nesokia-head -r -o "table" -f "prg_ram=8192&chr_rom>4000" -k "mapper&prg_rom&chr_rom&prg_ram&chr_ram&mirroring" ./path
```

List only mapper and PRG ROM size, in JSON:

```bash
nesokia-head -k "mapper&prg_rom" -o json "Super C (U) .nes" "Trog (U) .nes"
```

Follow symlinks, NES 2.0 only, pretty Markdown:

```bash
nesokia-head -o md "Zen - Intergalactic Ninja (U) .nes"
```

---

## License & building

Copyright (C) 2025 Codemeow

License: MIT <https://opensource.org/license/MIT>    
Permission is hereby granted, free of charge, to any person obtaining a copy    
of this software and associated documentation files (the "Software"), to deal    
in the Software without restriction, including without limitation the rights    
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell    
copies of the Software, subject to the above copyright notice and this    
permission notice being included in all copies or substantial portions of    
the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
