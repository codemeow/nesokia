; @file nsk_header_prgram.inc
; @brief Include module for the PRG-(NV)RAM size
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_RAM_PRG__
::__NSK_HEADER_RAM_PRG__ = 1

.include "../../memory/ram/nsk_header_ram.inc"

; @macro nsk_header_prgram
; @brief Build NES 2.0 header byte for PRG-RAM and PRG-NVRAM sizes.
;
; Both sizes must be multiples of 64 and encodeable as
; (64 << nibble). A value of 0 means “not present”.
;
; @param    prgram_size   : Volatile PRG-RAM (work RAM) size in bytes.
; @param    prgnvram_size : Non-volatile PRG-NVRAM (battery-backed) size in bytes.
.macro nsk_header_prgram prgram_size, prgnvram_size
    .local call
    call:
        nsk_header_ram prgram_size, prgnvram_size
.endmacro

.endif
