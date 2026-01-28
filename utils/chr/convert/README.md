# nesokia-chr-convert

Template-based CHR conversion utility for NES PPU data.

---

## Overview

`nesokia-chr-convert` converts between PNG-based templates and NES CHR-related binary data.
It supports two conversion directions:

* **PNG Template(s) to CHR**
* **CHR to PNG Template(s)**

Each mode produces or consumes PPU-compatible data structures, including palettes and pattern tables.

---

## Conversion Modes

### PNG Template to CHR

In this mode, a PNG template (or a set of template components) is converted into one or more of the following outputs:

* **PPU color table**
  * Format reference: [https://www.nesdev.org/wiki/.pal](https://www.nesdev.org/wiki/.pal)
* **Selected palettes (single plane)**
  * Intended for loading into `$3f00–$3f0f` *or* `$3f10–$3f1f`
* **Selected palettes (both planes)**
  * Intended for loading into `$3f00–$3f1f`
* **Left pattern table**
  * Intended for loading into `$0000–$0fff`
* **Right pattern table**
  * Intended for loading into `$1000–$1fff`
* **Both pattern tables**
  * Intended for loading into `$0000–$1fff`

---

### CHR to PNG Template

In this mode, CHR-related input files are converted into:

* a **single PNG template**, or
* a **set of PNG template components**.

Example template layout: [2c02-template.png](templates/user/2c02-template.png)

---

## Usage

```
nesokia-chr-convert <mode> <ppuflags> <input options> <output options>
```

---

## Mode Selection

Exactly **one** conversion mode must be specified.

```
-M, --mode=t2c
```
Template to CHR conversion (default)

```
-M, --mode=c2t
```
CHR to Template conversion

---

## PPU Pattern Table Configuration

Exactly **one** PPU configuration must be specified.

```
-A, --back-address=0000
```
The **left** (`$0000`) pattern table is assigned to **background** tiles    
The **right** (`$1000`) pattern table is assigned to **sprite** tiles    
(*PPUCTRL* bits: `...0 1...`) (default)

```
-A, --back-address=1000
```
The **left** (`$0000`) pattern table is assigned to **sprite** tiles    
The **right** (`$1000`) pattern table is assigned to **background** tiles    
(*PPUCTRL* bits: `...1 0...`)

---

## PNG Template to CHR Conversion

### Input Options

Input must be provided **either** as a single combined PNG template **or** as separate template components.
These options are **mutually exclusive**.

#### Combined template input

```
-i, --input <template.png>
```

#### Separate template components

```
-c, --input-ppu-colors <colors.png>
-p, --input-palettes <palettes.png>
-l, --input-left-table <left.png>
-r, --input-right-table <right.png>
```

---

### Output Options

##### Combined palette output

```
-P, --output-palettes <palettes file>
```

##### Separate palette planes

```
-B, --output-palette-back <background file>
-S, --output-palette-sprites <sprites file>
```

#### Pattern tables

##### Combined pattern tables

```
-T, --output-pattern-tables <tables file>
```

##### Separate pattern tables

```
-L, --output-left-table <left file>
-R, --output-right-table <right file>
```

#### PPU colors/palette

PPU color output:

```
-C, --output-ppu-colors <palette file>
```

#### Notes

* File extensions are **not validated**; any extension (or none) may be used.
* All output files are optional.

---

## CHR to PNG Template Conversion

### Input Options

#### Palettes

Palette input must be provided **either** as a combined file **or** as separate planes.
These options are **mutually exclusive**.

##### Combined palette input

```
-p, --input-palettes <palettes file>
```

##### Separate palette planes

```
-b, --input-palette-back <background file>
-s, --input-palette-sprites <sprites file>
```

#### Pattern tables

Pattern table input must be provided **either** as a combined file **or** as separate tables.
These options are **mutually exclusive**.

##### Combined pattern tables

```
-t, --input-pattern-tables <tables file>
```

##### Separate pattern tables

```
-l, --input-left-table <left file>
-r, --input-right-table <right file>
```

#### PPU colors/palette

PPU color input:

```
-c, --input-ppu-colors <palette file>
```

---

### Output Options

#### Combined template output

```
-O, --output <template.png>
```

#### Separate template components

```
-C, --output-ppu-colors <colors.png>
-P, --output-palettes <palettes.png>
-L, --output-left-table <left.png>
-R, --output-right-table <right.png>
```

#### Notes

* All output files are optional.

---

## Examples

### Convert a template into palettes and pattern tables

```
nesokia-chr-convert     \
                        \
  -i template.png       \
                        \
  -C colors.pal         \
  -P palettes.spals     \
  -T patterns.pats
```

---

### Convert a template into separate pattern tables, explicitly stating the conversion direction

```
nesokia-chr-convert     \
  -M t2c                \
                        \
  -i template.png       \
                        \
  -L left.pat           \
  -R right.pat
```

---

### Convert CHR data into a single PNG template

```
nesokia-chr-convert     \
  -M c2t                \
                        \
  -c colors.pal         \
  -p palettes.spals     \
  -t patterns.pats      \
                        \
  -O template.png
```

---

### Convert CHR data into separate PNG components

```
nesokia-chr-convert     \
  -M c2t                \
                        \
  -c colors.pal         \
  -b back.spal          \
  -s sprites.spal       \
  -l left.pat           \
  -r right.pat          \
                        \
  -C colors.png         \
  -P palettes.png       \
  -L left.png           \
  -R right.png
```

### Do not convert anything, but validate input template
```
nesokia-chr-convert     \
                        \
    -i template.png
```

### Same as above, but the left table contains sprites data
```
nesokia-chr-convert     \
                        \
    -A 1000             \
                        \
    -i template.png
```
---

## Disallowed PPU Colors in Templates

Both combined templates and individual template components may explicitly mark
**disallowed PPU colors**.

A color is considered *disallowed* when a single PPU palette color cell contains
**more than one distinct color**.
This mechanism is intended to represent colors that are invalid or unavailable on
a given PPU variant (for example, color `$0D` on the NES PPU 2C02).

In practice, a disallowed color is typically indicated by drawing a visual marker 
- such as a cross or overlay-using a second color within the same palette cell.
The specific visual appearance is not significant; only the presence of multiple
colors within the cell is used to signal that the palette entry is invalid.

During conversion, such palette entries are treated as **explicitly prohibited**
and will not be assigned to output palettes or used in validation.

An example of this technique can be seen in the reference template: 
[2c02-template.png](templates/user/2c02-template.png)

---

## Use of Separate Template Components

In addition to a single combined PNG template, the converter supports the use of
separate PNG template components as input.

This mode is intended for workflows where different CHR elements are managed or 
generated independently. A common example is a ROM that uses CHR bank switching, 
where pattern tables are divided into multiple banks that are loaded or replaced 
at runtime. In such cases, maintaining a single monolithic template may be 
inconvenient or impractical.

By providing template components separately - such as individual palette definitions 
and left/right pattern tables - each CHR bank can be edited, versioned, and processed 
in isolation. This allows changes to be made to one bank without affecting others 
and avoids the need to merge multiple banks into a single full template image.

Functionally, the conversion result is equivalent to that produced from a combined 
template. The choice between a full template and separate components is purely a 
matter of workflow and project organization.

---

## Quick Options Reference Sheet

### Common arguments

| Option                            | Argument      | Description                           |
| :-------------------------------- | :------------ | :------------------------------------ |
| `-A, --back-address`              | `0000`/`1000` | Background pattern table address      |
|                                   |               | Default is `0000`                     |

### PNG template to CHR

#### Input (PNG)

| Option                            | Argument      | Description                           |
| :-------------------------------- | :------------ | :------------------------------------ |
| `-M, --mode`                      | `t2c`         | Select template to CHR mode (default) |
| `-i, --input`                     | `<file.png>`  | Input combined PNG template           |
| `-c, --input-ppu-colors`          | `<file.png>`  | Input PPU color table                 |
| `-p, --input-palettes`            | `<file.png>`  | Input palettes table                  |
| `-l, --input-left-table`          | `<file.png>`  | Input left table                      |
| `-r, --input-right-table`         | `<file.png>`  | Input right table                     |

#### Output (CHR)

| Option                            | Argument      | Description                           |
| :-------------------------------- | :------------ | :------------------------------------ |
| `-C, --output-ppu-colors`         | `<file>`      | Output PPU colors (`.pal`)            |
| `-P, --output-palettes`           | `<file>`      | Output palettes `$3f00–$3f1f`         |
| `-B, --output-palette-back`       | `<file>`      | Output palette  `$3f00–$3f0f`         |
| `-S, --output-palette-sprites`    | `<file>`      | Output palette  `$3f10–$3f1f`         |
| `-T, --output-pattern-tables`     | `<file>`      | Output pattern tables `$0000–$1fff`   |
| `-L, --output-left-table`         | `<file>`      | Output pattern table  `$0000–$0fff`   |
| `-R, --output-right-table`        | `<file>`      | Output pattern table  `$1000–$1fff`   |

### CHR to PNG template

#### Input (CHR)

| Option                            | Argument      | Description                           |
| :-------------------------------- | :------------ | :------------------------------------ |
| `-M, --mode`                      | `c2t`         | Select CHR to template mode           |
| `-c, --input-ppu-colors`          | `<file>`      | Input PPU colors (`.pal`)             |
| `-p, --input-palettes`            | `<file>`      | Input palettes `$3f00–$3f1f`          |
| `-b, --input-palette-back`        | `<file>`      | Input palette  `$3f00–$3f0f`          |
| `-s, --input-palette-sprites`     | `<file>`      | Input palette  `$3f10–$3f1f`          |
| `-t, --input-pattern-tables`      | `<file>`      | Input pattern tables `$0000–$1fff`    |
| `-l, --input-left-table`          | `<file>`      | Input pattern table  `$0000–$0fff`    |
| `-r, --input-right-table`         | `<file>`      | Input pattern table  `$1000–$1fff`    |

#### Output (PNG)

| Option                            | Argument      | Description                           |
| :-------------------------------- | :------------ | :------------------------------------ |
| `-O, --output`                    | `<file.png>`  | Output combined PNG template          |
| `-C, --output-ppu-colors`         | `<file.png>`  | Output PNG PPU colors                 |
| `-P, --output-palettes`           | `<file.png>`  | Output PNG palettes                   |
| `-L, --output-left-table`         | `<file.png>`  | Output PNG left pattern table         |
| `-R, --output-right-table`        | `<file.png>`  | Output PNG right pattern table        |

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
