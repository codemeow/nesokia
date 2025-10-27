; @file nsk_header_rom_misc.inc
; @brief Include module for the miscellaneous ROMs count byte
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_ROM_MISC__
::__NSK_HEADER_ROM_MISC__ = 1

.linecont +

; @macro _convert_roms_misc
; @brief Convert the number of Miscellaneous ROM chips into header bits.
;
; This macro validates and assigns the number of miscellaneous ROMs
; (additional ROM areas not covered by PRG/CHR) to an output symbol.
;
; @param    roms        Number of miscellaneous ROM chips (0–3).
; @param    out_bits    Symbol to receive the converted value.
;
; @error Emits an error if @p roms is outside the valid range
.macro _convert_roms_misc roms, out_bits
    .if roms >= 0 && roms <= 3
        out_bits = roms
        
    .else
        .error "Invalid value: NSK_HEADER_ROMS_MISC"
    .endif
.endmacro

; @macro nsk_header_rom_misc
; @brief Write the Miscellaneous ROM count byte into the NES 2.0 header.
;
; Typical values:
; - 0 = no misc ROM
; - 1 = one misc ROM (e.g., VT369 internal ROM, Mapper 086 speech ROM)
; - 2 = two misc ROMs
; - 3 = three misc ROMs (e.g., PlayChoice-10 INST ROM + 2 PROMs)
;
; ### Known cases of Miscellaneous ROM usage (NES 2.0 spec):
; | Value | Example system / mapper     | Description                                               |
; |-------|-----------------------------|-----------------------------------------------------------|
; | 1     | VT369                       | 4 KiB internal ROM embedded in SoC                        |
; | 1     | Mapper 086 (submapper 1)    | Extra ROM containing speech data                          |
; | 1     | Mapper 355                  | Internal ROM of PIC16C54 microcontroller (protection)     |
; | 1     | Mapper 561 / 562            | Nonstandard trainers (not 512 B, not at $7000)            |
; | 3     | PlayChoice-10               | INST-ROM (8 KiB) + PROM Data (16 B) + PROM Counter (16 B) |
;
; @param    roms_misc   Number of miscellaneous ROM chips (0–3).
;
; @note This field is only meaningful for mappers/consoles that define
;       how the misc area is interpreted.
.macro nsk_header_rom_misc roms_misc
    ; locals:
    .local bits_roms

    ; convert:
    _convert_roms_misc roms_misc, bits_roms

    ; write:
    .byte bits_roms
.endmacro

.endif
