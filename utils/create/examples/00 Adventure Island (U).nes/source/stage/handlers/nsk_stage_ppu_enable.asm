; @file nsk_stage_ppu_enable.asm
; @brief Source module for the PPU enable stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_PPU_ENABLE_ASM__
::__NSK_STAGE_PPU_ENABLE_ASM__ = 1

.include "../../stage/handlers/nsk_stage_ppu_enable.inc"
.include "../../stage/nsk_stage_list.inc"
.include "../../ppu/nsk_ppu_render.inc"
.include "../../ppu/nsk_ppu_nmi.inc"

.segment "CODE"

; @brief Stage for PPU enabling
.export nsk_stage_ppu_enable
.proc nsk_stage_ppu_enable
    jsr nsk_ppu_render_enable
    jsr nsk_ppu_nmi_enable

    jsr nsk_stage_next
    rts
.endproc

.endif