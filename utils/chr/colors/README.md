# nesokia-chr-colors

PPU colors conversion tool

## Overview

Converts PPU colors from and to different formats:

From
- .png file (Nesokia PNG template file)
- .pal file ([format description](https://www.nesdev.org/wiki/.pal)

To
- ANSI-colored text output
- .png file (Nesokia PNG template file)
- .pal file ([format description](https://www.nesdev.org/wiki/.pal)
- .gpl file (GIMP palette)
- .ase file (Adobe Swatch Exchange)
- .aco file (Photoshop Color Swatch)

---

## Usage

`nesokia-chr-colors -i <inputfile> -o <outputfile>`

By default the input and output formats are deduced by the file extension.
This could be overrided with the `-I`/`--input-format` and `-O`/`--output-format` flags:

Input formats:
```
-I "png"
-I "pal"
```
Output formats:
```
-O "png"
-O "pal"
-O "gpl"
-O "ase"
-O "aco"
```
For example:
```
nesokia-chr-colors -i input.palette -I pal -o output.gimp -O gpl
```
- Input file "input.palette" is interpreted as ".pal" file
- Output file "output.gimp" is written in ".gpl" file format

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
