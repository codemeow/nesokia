; @file nsk_stage_ppu_disable.asm
; @brief Source module for the PPU disable stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_PPU_DISABLE_ASM__
::__NSK_STAGE_PPU_DISABLE_ASM__ = 1

.include "../../stage/handlers/nsk_stage_ppu_disable.inc"
.include "../../stage/nsk_stage_list.inc"
.include "../../ppu/nsk_ppu_render.inc"
.include "../../ppu/nsk_ppu_nmi.inc"

.segment "CODE"

; @brief Stage for PPU disabling
.export nsk_stage_ppu_disable
.proc nsk_stage_ppu_disable
    jsr nsk_ppu_render_disable
    jsr nsk_ppu_nmi_disable

    jsr nsk_stage_next
    rts
.endproc

.endif