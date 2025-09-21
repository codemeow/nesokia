# Header Generator

The **Header Generator** module provides a configurable way to generate **NES 2.0 headers** for ROM images.  
It is designed to be flexible and easily integrated into projects built with **ca65**.

---

## Features

- Generates NES 2.0 headers based on user-defined configuration parameters.
- Provides a sample configuration file for quick setup.
- Automatically generates required constants and flags.
- Supports customization of the output segment name for integration.
- Includes enums and constants for use in your assembly projects.

---

## Installation & Setup

1. Copy the header directory into your project. The name of the copied directory doesn't matter.

2. Change the `nsk_header_config.inc` configuration file.  
    * Refer to `header/nsk_header_consts.inc` for available constant values when editing the configuration file.
    * Refer to `header/nsk_header_config.sample.inc` for sample configuration

3. Add the directory to your build system, ensuring that `header/nsk_header_code.asm` is compiled and linked into your project.

---

## Usage

* The **`nsk_header_config.inc`** file controls all parameters of the generated header.
* Constants and flags are generated automatically, depending on the values you set.
* The configuration file also allows you to specify the name of the segment used to integrate the generated code.

Example (from the sample configuration file):

```asm
; @brief Release Region
;
; Sets the region that the emulator should use when launching the ROM image.
; See #nsk_header_regions in `header/nsk_header_consts.inc` for a list of possible values.
::NSK_HEADER_REGION = nsk::RP2C02
```

This directive defines the release region for the ROM by referencing constants provided in `header/nsk_consts.inc`.

---

## License

This module is part of the **Nesokia** project and is licensed under the **MIT License**.
See the [LICENSE](../LICENSE) file for details.


