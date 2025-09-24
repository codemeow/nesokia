; @file nsk_header_prg_lsb.inc
; @brief Include module for the PRG-ROM size LSB
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_PRGROM_LSB__
::__NSK_HEADER_PRGROM_LSB__ = 1

.linecont +

.include "nsk_header_prgrom_consts.inc"
.include "nsk_header_rom_issimple.inc"
.include "nsk_header_rom_lsb_exponent.inc"

; @macro _prgrom_lsb_simple
; @brief Compute PRG-ROM size LSB using simple (linear) encoding.
;
; Encodes the given PRG-ROM size as a count of 16 KiB units
; (`::NSK_HEADER_PRGROM_QUANTUM`) and extracts the low byte.
;
; @param    prgrom_size  ROM size in bytes (constant expression).
; @param    out_result   Symbol that will be set to the resulting LSB value.
.macro _prgrom_lsb_simple prgrom_size, out_result
    calc:
        out_result = (prgrom_size / ::NSK_HEADER_PRGROM_QUANTUM) & $ff
.endmacro

; @macro _prgrom_lsb_exponent
; @brief Compute PRG-ROM size LSB using exponent–multiplier encoding.
;
; Delegates the calculation to `nsk_header_romlsb_exponent`, which
; implements the NES 2.0 exponent–multiplier scheme.
;
; @param    prgrom_size  ROM size in bytes (constant expression).
; @param    out_result   Symbol that will be set to the resulting LSB value.
.macro _prgrom_lsb_exponent prgrom_size, out_result
    call:
        nsk_header_romlsb_exponent prgrom_size, out_result
.endmacro

; @macro nsk_header_prgrom_lsb
; @brief Write PRG-ROM size LSB into the header (auto-select encoding).
;
; Determines whether the given PRG-ROM size can be represented
; with simple (linear) encoding or requires exponent–multiplier
; encoding. Calls the appropriate helper macro and writes the
; resulting LSB byte.
;
; @param    prgrom_size  ROM size in bytes (constant expression).
.macro nsk_header_prgrom_lsb prgrom_size
    locals:
        .local byte_prg
        .local flag_issimple_prg

    check:
        nsk_header_rom_issimple             \
            prgrom_size,                    \
            ::NSK_HEADER_PRGROM_QUANTUM,    \
            flag_issimple_prg

    call:
        .if flag_issimple_prg = 1
            _prg_lsb_simple prgrom_size, byte_prg
        .else
            _prg_lsb_exponent prgrom_size, byte_prg
        .endif

    write:
        .byte byte_prg
.endmacro

.delmacro _prgrom_lsb_simple
.delmacro _prg_lsb_exponent

.endif

