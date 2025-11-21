; @file nsk_stage_tiles_draw.asm
; @brief Source module for the tiles drawing stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_TILES_DRAW_ASM__
::__NSK_STAGE_TILES_DRAW_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../stage/handlers/nsk_stage_tiles_draw.inc"
.include "../../stage/nsk_stage_list.inc"

.segment "CODE"

; @brief Stage for tiles drawing
; @warning Uses raw OAM access, requires rendering/NMI to be disabled
.export nsk_stage_tiles_draw
.proc nsk_stage_tiles_draw
    ; @todo

    jsr nsk_stage_next
    rts
.endproc

.endif