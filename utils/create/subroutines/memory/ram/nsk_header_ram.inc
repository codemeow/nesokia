; @file nsk_header_log2.inc
; @brief Include module for the RAM-encoding
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_RAM__
::__NSK_HEADER_RAM__ = 1

.linecont +

.include "../../memory/ram/nsk_header_consts.inc"
.include "../../nsk_header_log2.inc"

; @macro _ram_sanity_check
; @brief Validate that a RAM size is aligned to the NES 2.0 encoding rules.
;
; NES 2.0 encodes RAM sizes as multiples of @ref NSK_HEADER_RAMSIZE_MULTIPLIER 
; This macro checks that @p ram_size is evenly divisible by the multiplier.
;
; @param    ram_size  RAM size in bytes to validate.
;
; @error Emits an error if @p ram_size is not a multiple of 64.
;
; ### Example
;   _ram_sanity_check 2048   ; OK (2 KiB is divisible by 64)
;   _ram_sanity_check 1234   ; ERROR
.macro _ram_sanity_check ram_size
    .local check
    check:
        .if ram_size .mod ::NSK_HEADER_RAMSIZE_MULTIPLIER <> 0
            .error                                                          \
                .sprintf (                                                  \
                    "Invalid RAM size: %d must be a multiple of %d",        \
                    ram_size,                                               \
                    ::NSK_HEADER_RAMSIZE_MULTIPLIER                         \
                )
        .endif
.endmacro

; @macro _ram_convert
; @brief Convert a RAM size in bytes into its 4-bit NES 2.0 encoding nibble.
;
; If @p ram_size = 0, assigns 0 to @p out_nib (no memory present).
; Otherwise divides @p ram_size by @ref NSK_HEADER_RAMSIZE_MULTIPLIER
; checks that the quotient is an exact power of two, and computes its base-2 
; logarithm
;
; @param    ram_size   RAM size in bytes (must be 0 or a valid NES 2.0 
;                      encodable size).
; @param    out_nib    Symbol assigned to the resulting nibble (0..15).
;
; @details The resulting nibble encodes the RAM size as:
;          size = 64 << out_nib
;
; ### Example
;   _ram_convert 2048, nib  ; nib = 5  (2 KiB = 64 << 5)
;   _ram_convert 0, nib     ; nib = 0  (no RAM)
.macro _ram_convert ram_size, out_nib
    .local convert
    convert:
        .if ram_size = 0
            out_nib = 0
        .else
            _pow2 .set ram_size / ::NSK_HEADER_RAMSIZE_MULTIPLIER
            nsk_header_log2 _pow2, out_nib
        .endif
.endmacro

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
    .local locals
    locals:
        .local nib_ram
        .local nib_nvram

    .local sanity
    sanity:
        _ram_sanity_check ram_size
        _ram_sanity_check nvram_size

    .local convert
    convert:
        _ram_convert    ram_size,      nib_ram
        _ram_convert    nvram_size,    nib_nvram

    .local write
    write:
        .byte                    \
            (nib_ram         ) | \
            (nib_nvram  <<  4)
.endmacro

.endif
