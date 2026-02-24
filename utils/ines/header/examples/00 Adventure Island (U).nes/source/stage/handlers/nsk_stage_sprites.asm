; @file nsk_stage_sprites.asm
; @brief Source module for the sprites drawing
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_SPRITES_ASM__
::__NSK_STAGE_SPRITES_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "../../stage/handlers/nsk_stage_sprites.inc"
.include "../../objects/sprites/pool/nsk_pool_tick.inc"

.segment "CODE"

; @brief Draw sprites
; @note In this demo no other levels are planned, so this
;    function is a common sprites drawer
.export nsk_stage_sprites_draw
.proc nsk_stage_sprites_draw
    jsr nsk_pool_tick
    rts
.endproc

.endif