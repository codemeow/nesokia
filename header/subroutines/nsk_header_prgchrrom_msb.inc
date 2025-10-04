; @file nsk_header_prgchrrom_msb.inc
; @brief Include module for the PRG/CHR-ROM size MSB
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_PRGCHRROM_MSB__
::__NSK_HEADER_PRGCHRROM_MSB__ = 1

.linecont +

.include "nsk_header_chrrom_consts.inc"
.include "nsk_header_prgrom_consts.inc"
.include "nsk_header_rom_issimple.inc"

; @macro _prg_msb_simple
; @brief Compute the high nibble of PRG-ROM size in simple encoding mode.
;
; @param    prgrom_size  PRG-ROM size in bytes.
; @param    out_nib      Symbol assigned to the 4-bit result.
.macro _prg_msb_simple prgrom_size, out_nib
.scope
    calc:
        out_nib = ((prgrom_size / ::NSK_HEADER_PRGROM_QUANTUM) >> 8) & $0f
.endscope
.endmacro

; @macro _chr_msb_simple
; @brief Compute the high nibble of CHR-ROM size in simple encoding mode.
;
; @param    chrrom_size  CHR-ROM size in bytes.
; @param    out_nib      Symbol assigned to the 4-bit result.
.macro _chr_msb_simple chrrom_size, out_nib
.scope
    calc:
        out_nib = ((chrrom_size / ::NSK_HEADER_CHRROM_QUANTUM) >> 8) & $0f
.endscope
.endmacro

; @macro _prg_msb_exponent
; @brief PRG-ROM high nibble placeholder in exponent–multiplier mode.
;
; When PRG-ROM size is encoded using exponent–multiplier encoding,
; the header’s PRG-ROM MSB field must be set to $F.
;
; @param    out_nib  Symbol assigned to $0F.
.macro _prg_msb_exponent out_nib
.scope
    set:
        out_nib = $0f
.endscope
.endmacro

; @macro _chr_msb_exponent
; @brief CHR-ROM high nibble placeholder in exponent–multiplier mode.
;
; When CHR-ROM size is encoded using exponent–multiplier encoding,
; the header’s CHR-ROM MSB field must be set to $F.
;
; @param    out_nib  Symbol assigned to $0F.
.macro _chr_msb_exponent out_nib
.scope
    set:
        out_nib = $0f
.endscope
.endmacro

; @macro nsk_header_prgchrrom_msb
; @brief Build the NES 2.0 header byte containing PRG-ROM and CHR-ROM MSBs.
;
; Determines whether @p prgrom_size and @p chrrom_size can be expressed
; in simple mode. For each ROM:
; - If simple encoding applies, computes the high nibble with
;   @_prg_msb_simple or @_chr_msb_simple.
; - Otherwise, sets the field to $F (exponent–multiplier mode).
;
; Packs the results into one byte:
;   bits 0–3 : PRG-ROM size MSB nibble
;   bits 4–7 : CHR-ROM size MSB nibble
;
; @param    prgrom_size  PRG-ROM size in bytes.
; @param    chrrom_size  CHR-ROM size in bytes.
.macro nsk_header_prgchrrom_msb prgrom_size, chrrom_size
.scope
    locals:
        .local nib_prg
        .local nib_chr
        .local flag_issimple_prg
        .local flag_issimple_chr

    check:
        nsk_header_rom_issimple             \
            prgrom_size,                    \
            ::NSK_HEADER_PRGROM_QUANTUM,    \
            flag_issimple_prg

        nsk_header_rom_issimple             \
            chrrom_size,                    \
            ::NSK_HEADER_CHRROM_QUANTUM,    \
            flag_issimple_chr

    call:
        .if flag_issimple_prg = 1
            _prg_msb_simple prgrom_size, nib_prg
        .else
            _prg_msb_exponent nib_prg
        .endif

        
        .if flag_issimple_chr = 1
            _chr_msb_simple chrrom_size, nib_chr
        .else
            _chr_msb_exponent nib_chr
        .endif

    write:
        .byte                    \
            (nib_prg         ) | \
            (nib_chr    <<  4)
.endscope
.endmacro

.delmacro _prg_msb_simple
.delmacro _chr_msb_simple
.delmacro _prg_msb_exponent
.delmacro _chr_msb_exponent

.endif

