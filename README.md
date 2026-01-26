# Nesokia

**Nesokia** is a collection of utilities designed to simplify routine tasks when working with NES assembly code using **ca65**.  
The project is structured as a set of submodules, each dedicated to a specific task.

## Current Modules

_Each module provides its own documentation inside its respective subdirectory._

- **Header Reader** (`utils/ines/inspect`)

  iNES/NES 2.0 headers reader

- **Header Generator** (`utils/ines/header`)

  NES 2.0 ROM header generator

- **Tiles converter** (`utils/chr/convert`)

  Image to CHR data/PAL data converter

- **Palette converter** (`utils/chr/colors`)

  Converts .pal files into composite component for `utils/chr/convert`
  * PNG component output
  * Terminal colored ANSI output
  * Terminal text output

### Todo

- **Easing data generator** (`utils/math/ease`)

  Generates the data tables for easing functions (ex. Doner tool)

## Authors

- [codemeow](https://github.com/codemeow)

## License

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)  
This project is licensed under the **MIT License**.  
See the [LICENSE](LICENSE) file for details. Some exceptions apply.
