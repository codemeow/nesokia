; @file nsk_header_chr_lsb.asm
; @brief Include module for the CHR-ROM size LSB
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_CHRROM_LSB__
::__NSK_HEADER_CHRROM_LSB__ = 1

.linecont +

.include "nsk_header_chrrom_consts.inc"
.include "nsk_header_rom_issimple.inc"
.include "nsk_header_rom_lsb_exponent.inc"

; @macro _chrrom_lsb_simple
; @brief Compute CHR-ROM size LSB using simple (linear) encoding.
;
; Encodes the given CHR-ROM size as a count of 16 KiB units
; (`::NSK_HEADER_CHRROM_QUANTUM`) and extracts the low byte.
;
; @param    chrrom_size  ROM size in bytes (constant expression).
; @param    out_result   Symbol that will be set to the resulting LSB value.
.macro _chrrom_lsb_simple chrrom_size, out_result
    calc:
        out_result = (chrrom_size / ::NSK_HEADER_CHRROM_QUANTUM) & $ff
.endmacro

; @macro _chrrom_lsb_exponent
; @brief Compute CHR-ROM size LSB using exponent–multiplier encoding.
;
; Delegates the calculation to `nsk_header_romlsb_exponent`, which
; implements the NES 2.0 exponent–multiplier scheme.
;
; @param    chrrom_size  ROM size in bytes (constant expression).
; @param    out_result   Symbol that will be set to the resulting LSB value.
.macro _chrrom_lsb_exponent chrrom_size, out_result
    call:
        nsk_header_romlsb_exponent chrrom_size, out_result
.endmacro

; @macro nsk_header_chrrom_lsb
; @brief Write CHR-ROM size LSB into the header (auto-select encoding).
;
; Determines whether the given CHR-ROM size can be represented
; with simple (linear) encoding or requires exponent–multiplier
; encoding. Calls the appropriate helper macro and writes the
; resulting LSB byte.
;
; @param    chrrom_size  ROM size in bytes (constant expression).
.macro nsk_header_chrrom_lsb chrrom_size
    locals:
        .local result
        .local issimple

    check:
        nsk_header_rom_issimple             \
            chrrom_size,                    \
            ::NSK_HEADER_CHRROM_QUANTUM,    \
            issimple

    call:
        .if issimple = 1
            _chr_lsb_simple chrrom_size, result
        .else
            _chr_lsb_exponent chrrom_size, result
        .endif

    write:
        .byte result
.endmacro

.endif

