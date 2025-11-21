; @file nsk_stage_sprites_draw.asm
; @brief Source module for the sprites drawing stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_SPRITES_DRAW_ASM__
::__NSK_STAGE_SPRITES_DRAW_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../stage/handlers/nsk_stage_sprites_draw.inc"
.include "../../stage/nsk_stage_list.inc"

.segment "CODE"

; @brief Stage for sprites drawing
; @warning Uses raw OAM access, requires rendering/NMI to be disabled
.export nsk_stage_sprites_draw
.proc nsk_stage_sprites_draw
    ; @todo

    jsr nsk_stage_next
    rts
.endproc

.endif