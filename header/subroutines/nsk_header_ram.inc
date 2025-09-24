; @file nsk_header_log2.inc
; @brief Include module for the RAM-encoding
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_RAM__
::__NSK_HEADER_RAM__ = 1

.linecont +

.include "nsk_header_ram_consts.inc"
.include "nsk_header_log2.inc"

; @macro nsk_header_ram
; @brief Build NES 2.0 header byte for volatile and non-volatile RAM sizes.
;
; Encodes general-purpose RAM and battery-backed NVRAM sizes into one header 
; byte using the NES 2.0 formula:
;
;     size = ::NSK_HEADER_RAMSIZE_MULTIPLIER << nibble
;
; @param    ram_size    Size of volatile RAM (in bytes).
; @param    nvram_size  Size of non-volatile RAM (NVRAM / EEPROM) in bytes.
.macro nsk_header_ram ram_size, nvram_size
    locals:
        .local nib_ram
        .local nib_nvram

    sanity:
        .if ram_size .mod ::NSK_HEADER_RAMSIZE_MULTIPLIER <> 0
            .error                                                          \
                .sprintf (                                                  \
                    "Invalid RAM size: %d must be a multiple of %d",        \
                    ram_size,                                               \
                    ::NSK_HEADER_RAMSIZE_MULTIPLIER                         \
                )
        .endif

        .if nvram_size .mod ::NSK_HEADER_RAMSIZE_MULTIPLIER <> 0
            .error                                                          \
                .sprintf (                                                  \
                    "Invalid NVRAM size: %d must be a multiple of %d",      \
                    nvram_size,                                             \
                    ::NSK_HEADER_RAMSIZE_MULTIPLIER                         \
                )
        .endif

    check:
        .if ram_size = 0
            nib_ram = 0
        .else
            _pow2 .set ram_size / ::NSK_HEADER_RAMSIZE_MULTIPLIER
            nsk_header_log2 _pow2, nib_ram
        .endif

        .if nvram_size = 0
            nib_nvram = 0
        .else
            _pow2 .set nvram_size / ::NSK_HEADER_RAMSIZE_MULTIPLIER
            nsk_header_log2 _pow2, nib_nvram
        .endif

    write:
        .byte                    \
            (nib_ram         ) | \
            (nib_nvram  <<  4)
.endmacro

.endif
