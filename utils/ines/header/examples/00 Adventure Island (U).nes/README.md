# Example 00: Adventure Island (U)

This is the simpliest example, standard console, simple PRG/CHR ROM combo, no 
battery or trainer area.

## General ROM info

### Console

* **Type:** Nintendo Entertainment System / Famicom / Dendy
* **Timings / region:** RP2C02 PPU / NTSC 21.47 MHz / North America / Japan / South Korea / Taiwan
* **Vs. PPU type:** Not applicable / Any RP2C03 / Any RC2C03
* **Vs. Hardware type:** Not applicable / Vs. Unisystem (normal)

### Nametables

* **Mirroring / layout:** Horizontal arrangment / Vertical mirroring
* **Alternative layout:** Normal nametables layout

### Mapper

* **Mapper ID:** 3
* **Submapper ID:** 2

### Memory

| Type | Size |
| ---- | ---- |
| PRG ROM | 32.00 KiB (32768 bytes) |
| CHR ROM | 32.00 KiB (32768 bytes) |
| PRG RAM | 0 bytes |
| CHR RAM | 0 bytes |
| PRG NVRAM | 0 bytes |
| CHR NVRAM | 0 bytes |
| Misc. ROMs | 0 |

### Miscellaneous info

* **Trainer area:** Not present
* **Non volatile memory:** Not present
* **Expansion device:** Standard Famicom/NES controllers

## Nesokia configuration file

`nsk_header_config.inc`:
```asm
; Mapper ID
::NSK_HEADER_MAPPER_ID = 3
; Submapper ID
::NSK_HEADER_SUBMAPPER_ID = 2

; Horizontal arrangment / Vertical mirroring
::NSK_HEADER_NAMETABLE_HARDWIRED = NSK::NM_LAYOUT::HORIZONTAL_ARRANGEMENT

; PRG ROM size
::NSK_HEADER_PRGROM_SIZE = 32768
; CHR ROM size
::NSK_HEADER_CHRROM_SIZE = 32768

; Standard Famicom/NES controllers
::NSK_HEADER_EXPANSION_DEVICE = NSK::DEVICES::NINTENDO::CONTROLLERS
```

Any non-listed values will be set as 0. Default header segment name will be set as "HEADER"

## Compilation

```
make
```
```

```
