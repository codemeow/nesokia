; @file nsk_header_chrram.inc
; @brief Include module for the CHR-(NV)RAM size
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_CHRRAM__
::__NSK_HEADER_CHRRAM__ = 1

.include "nsk_header_ram.inc"

; @macro nsk_header_prgram
; @brief Build NES 2.0 header byte for CHR-RAM and CHR-NVRAM sizes.
;
; Both sizes must be multiples of 64 and encodeable as
; (64 << nibble). A value of 0 means “not present”.
;
; @param    prgram_size   : Volatile CHR-RAM size in bytes.
; @param    prgnvram_size : Non-volatile CHR-NVRAM (battery-backed) size in bytes.
.macro nsk_header_chrram chrram_size, chrnvram_size
.scope
    call:
        nsk_header_ram chrram_size, chrnvram_size
.endscope
.endmacro

.endif
