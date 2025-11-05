; @file nsk_header_config.sample.inc
; @brief Sample configuration for NES 2.0 header generation.
;
; Provides sample configuration with additional comments
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_CONFIG__
::__NSK_HEADER_CONFIG__ = 1

; @brief Selects the segment where the linker should place the
; generated header.
;
; This requires the header to be defined in the linker script as follows:
; ~~~
; MEMORY {
;    HEADER: start = $00, size = $10, type = rw, fill = yes, fillval = $00;
;    ...
; }
;
; SEGMENTS {
;    HEADER: load = HEADER, type = ro, align = $10;
;    ...
; }
; ~~~
;
; Any name can be used in the linker script, but the variable must match
; the *segment* name.
;
; Common values:
;  - "HEADER"
;  - "header"
;
.define NSK_SEGMENT_HEADER "HEADER"

; @brief Console type
;
; Defines the target console type
;
; See `NSK::CONSOLE` in `nsk_header_consts.inc` for a list of possible values.
;
; Common values:
;   - NSK::CONSOLE::FAMICOM
;   - NSK::CONSOLE::VRTECHNOLOGY_VT03
;   - NSK::CONSOLE::NINTENDO_VS
;
::NSK_HEADER_CONSOLE_TYPE = NSK::CONSOLE::FAMICOM

; @brief Release Region
;
; Specifies the target region (which affects CPU and PPU timing behavior) that
; the emulator should use when launching the ROM image.
;
; See `NSK::REGION` in `nsk_header_consts.inc` for a list of possible values.
;
; Common values:
;   NSK::REGION::NTSC
;   NSK::REGION::PAL
;   NSK::REGION::MULTIPLE
;
::NSK_HEADER_REGION = NSK::REGION::JAPAN

; @brief Mapper ID
;
; Defines the mapper number.
;
; @note Although often written with leading zeros (eg. 003) this value should
; be written in ca65-supported form: binary, decimal or hex (eg. `%3`, `3`, `$3`)
;
; Common values:
;   - 4 (MMC3)
;   - 0 (NROM)
;   - 1 (MMC1)
::NSK_HEADER_MAPPER_ID = 4

; @brief Submapper ID
;
; Defines the submapper ID
;
; Common values:
;   - 0
;   - 2 (e.g. UxROM with bitwise AND for bus conflicts)
;   - 1 (e.g. Mapper 562 with UN1ROM+CHRSW banking mode)
;
::NSK_HEADER_SUBMAPPER_ID = 0

; @brief Nametables layout
;
; Defines the hardwired nametables layout/mirroring
;
; See `NSK::NM_LAYOUT` in `nsk_header_consts.inc` for a list of possible values.
;
; Common values:
;   - NSK::NM_LAYOUT::MAPPER_CONTROLLED
;   - NSK::NM_LAYOUT::HORIZONTAL_MIRRORING
;   - NSK::NM_LAYOUT::VERTICAL_MIRROIRNG
;
::NSK_HEADER_NAMETABLE_HARDWIRED = NSK::NM_LAYOUT::MAPPER_CONTROLLED

; @brief Alternative nametables layout flag
;
; Defines if the `NSK_HEADER_NAMETABLE_HARDWIRED` should be treated
; differently.
;
; | Mapper |      NM_LAYOUT         | NM_ALTERNATIVE | Result                  |
; |--------|------------------------|----------------|-------------------------|
; |    030 | VERTICAL_ARRANGEMENT   | NORMAL         | Vertical arrangement    |
; |    030 | HORIZONTAL_ARRANGEMENT | NORMAL         | Horizontal arrangement  |
; |    030 | VERTICAL_ARRANGEMENT   | ALTERNATIVE    | 1 screen mode¹          |
; |    030 | HORIZONTAL_ARRANGEMENT | ALTERNATIVE    | 4 screen mode¹          |
; |    218 | VERTICAL_ARRANGEMENT   | NORMAL         | Vertical arrangement    |
; |    218 | HORIZONTAL_ARRANGEMENT | NORMAL         | Horizontal arrangement  |
; |    218 | VERTICAL_ARRANGEMENT   | ALTERNATIVE    | BLK0, single screen     |
; |    218 | HORIZONTAL_ARRANGEMENT | ALTERNATIVE    | BLK1, single screen     |
; |  other | VERTICAL_ARRANGEMENT   | NORMAL         | Vertical arrangement    |
; |  other | HORIZONTAL_ARRANGEMENT | NORMAL         | Horizontal arrangement  |
; |  other | VERTICAL_ARRANGEMENT   | ALTERNATIVE    | 4 screen mode²          |
;
; - ¹ For submappers 0, 1 and 4
; - ² When supported
;
; Common values:
;   - NSK::NM_ALTERNATIVE::NORMAL
;   - NSK::NM_ALTERNATIVE::ALTERNATIVE
;
::NSK_HEADER_NAMETABLE_ALTERNATIVE = NSK::NM_ALTERNATIVE::NORMAL

; @brief Battery-backed or other non-volatile memory
;
; Common values:
;   - NSK::BATTERY::NOT_PRESENT
;   - NSK::BATTERY::PRESENT
;
::NSK_HEADER_BATTERY = NSK::BATTERY::NOT_PRESENT

; @brief Trainer area
;
; The Trainer Area follows the 16-byte Header and precedes the PRG-ROM area.
; It is always 512 bytes in size if present, and contains data to be loaded
; into CPU memory at $7000.
;
; Common values:
;   - NSK::TRAINER::NOT_PRESENT
;   - NSK::TRAINER::PRESENT
;
::NSK_HEADER_TRAINER = NSK::TRAINER::NOT_PRESENT

; @brief PRG ROM in bytes
;
; @note The size must be divisible by 16,384 (16 KB), or representable as
; 2^X × M, where M ∈ {1, 3, 5, 7} and X ∈ [0, 63].
;
; Common values:
;   - 131072
;   - 262144
;   - 32768
;   - 524288
;
::NSK_HEADER_PRGROM_SIZE = 131072

; @brief CHR ROM in bytes
;
; @note The size must be divisible by 8,192 (8 KB), or representable as
; 2^X × M, where M ∈ {1, 3, 5, 7}.
;
; Common values:
;   - 131072
;   - 8192
;   - 262144
;   - 32768
;
::NSK_HEADER_CHRROM_SIZE = 131072

; @brief PRG RAM in bytes
;
; @note The size must be a power-of-two multiple of 64 bytes:
; size = 64 × 2^X, with X ∈ [0, 15] ∩ ℤ (equivalently, size = 64 << X).
;
; Set to zero when not used
;
; Common values:
;   - 0
;   - 8192
;   - 2048
;   - 4096
;
::NSK_HEADER_PRGRAM_SIZE = 0

; @brief CHR RAM in bytes
;
; @note The size must be a power-of-two multiple of 64 bytes:
; size = 64 × 2^X, with X ∈ [0, 15] ∩ ℤ (equivalently, size = 64 << X).
;
; Set to zero when not used
;
; Common values:
;   - 0
;   - 8192
;   - 32768
;   - 131072
;
::NSK_HEADER_CHRRAM_SIZE = 0

; @brief PRG NVRAM (EEPROM) in bytes
;
; @note The size must be a power-of-two multiple of 64 bytes:
; size = 64 × 2^X, with X ∈ [0, 15] ∩ ℤ (equivalently, size = 64 << X).
;
; Set to zero when not used
;
; Common values:
;   - 0
;   - 8192
;   - 32768
;   - 128
;
::NSK_HEADER_PRGNVRAM_SIZE = 0

; @brief CHR NVRAM in bytes
;
; @note The size must be a power-of-two multiple of 64 bytes:
; size = 64 × 2^X, with X ∈ [0, 15] ∩ ℤ (equivalently, size = 64 << X).
;
; Set to zero when not used
;
; Common values:
;   - 0
;   - 32768
;
::NSK_HEADER_CHRNVRAM_SIZE = 0

; @brief Additional ROMs
;
; If present, this section follows the CHR-ROM area and occupies the
; remaining space of the file.
;
; Common values:
;   - 0  (none)
;   - 3  (PlayChoice-10)
;   - 1  (VT369; mappers 086, 355, 561, and 562)
;
::NSK_HEADER_ROMS_MISC = 0

; @brief Vs. System PPU type
;
; @note This field is relevant only if `NSK_HEADER_CONSOLE_TYPE` is set to
;       `NSK::CONSOLE::NINTENDO_VS`. Otherwise, it is recommended to set the
;       value to `NSK::VSPPU::NONE`.
;
; Common values:
;   - NSK::VSHARDWARE::NONE
;   - NSK::VSHARDWARE::RP2C04_001
;   - NSK::VSHARDWARE::RP2C04_002
;
::NSK_HEADER_VS_PPU = NSK::VSPPU::NONE

; @brief Vs. System Hardware type
;
; @note This field is relevant only if `NSK_HEADER_CONSOLE_TYPE` is set to
;       `NSK::CONSOLE::NINTENDO_VS`. Otherwise, it is recommended to set the
;       value to `NSK::VSHARDWARE::NONE`.
;
; Common values:
;   - NSK::VSHARDWARE::NONE
;   - NSK::VSHARDWARE::VSUNI_NORMAL
;   - NSK::VSHARDWARE::VSDUAL_NORMAL
;
::NSK_HEADER_VS_HARDWARE = NSK::VSHARDWARE::NONE

; @brief Default expansion device
;
; Indicates that the ROM expects a specific set of devices accessible at CPU
; $4016/$4017. In almost all cases, this byte will specify the device without
; which the game cannot be played at all.
;
; Common values:
;   - NSK::DEVICES::NINTENDO::CONTROLLERS
;   - NSK::DEVICES::NONE::MULTICART
;   - NSK::DEVICES::NINTENDO::ZAPPER_P4017
;   - NSK::DEVICES::NINTENDO::FOURSCORE
;
::NSK_HEADER_EXPANSION_DEVICE = NSK::DEVICES::NINTENDO::CONTROLLERS

.endif