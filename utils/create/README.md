# Header Generator

The **Header Generator** module provides a configurable way to generate **NES 2.0 headers** for ROM images.  
It is designed to be flexible and easily integrated into projects built with **ca65**.

---

## Features

- Generates NES 2.0 headers based on user-defined configuration parameters.
- Provides a sample configuration file for quick setup.
- Supports customization of the output segment name for integration.
- Does not require copying the source files of this project into yours, thus keeping the projects separate. Only the configuration file is needed.

---

## Installation & Setup

### Installation

Either:
  1. Copy the `nsk_header_config.sample.inc` to your project
  2. Rename copied file to `nsk_header_config.inc`
  3. Change the values in the file according to your needs

Or:
  1. Add empty `nsk_header_config.inc` file to your project
  2. Populate it with the values, listed in the `nsk_header_config.sample.inc`

### Compilation

When using `ca65`:

  1. Pass the `-I XXX` option to the compiler, listing the directory, containing `nsk_header_config.inc`
  2. Use the `-o` option to select the output filename, containing the object file with the generated header
  3. Link the produced file with your project

---

## Usage

* The user-defined **`nsk_header_config.inc`** file controls all parameters of the generated header.

Example (from the sample configuration file):

```asm
; @brief Release Region
;
; Specifies the target region (affecting CPU/PPU timings) that the emulator
; should use when launching the ROM image.
;
; See `NSK::REGION` in `nsk_header_consts.inc` for a list of possible values.
;
; Commonly used values:
;   NSK::REGION::NTSC
;   NSK::REGION::PAL
;   NSK::REGION::MULTIPLE
;
::NSK_HEADER_REGION = NSK::REGION::JAPAN
```

---

## Updating the consts file

If, by some reason, you have changed any file in the `utils/common` directory, call the `utils/create/scripts/nsk_consts_update.sh` script to update the `nsk_header_consts.inc` accordingly.

---

## License

This module is part of the **Nesokia** project and is licensed under the **MIT License**.
See the [LICENSE](../LICENSE) file for details.


