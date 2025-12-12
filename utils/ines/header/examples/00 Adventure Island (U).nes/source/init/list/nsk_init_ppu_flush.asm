; @file nsk_init_ppu_flush.asm
; @brief Source module for the PPU flush init stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_INIT_PPU_FLUSH_ASM__
::__NSK_INIT_PPU_FLUSH_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../init/list/nsk_init_ppu_flush.inc"
.include "../../ppu/nsk_ppu_flush.inc"

.segment "CODE"

; @brief Init stage for PPU flushing
.export nsk_init_ppu_flush
.proc nsk_init_ppu_flush
    jsr nsk_ppu_flush_control
    jsr nsk_ppu_flush_mask

    rts
.endproc

.endif