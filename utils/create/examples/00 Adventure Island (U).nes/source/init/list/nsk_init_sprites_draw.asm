; @file nsk_init_sprites_draw.asm
; @brief Source module for the sprites drawing init stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_INIT_SPRITES_DRAW_ASM__
::__NSK_INIT_SPRITES_DRAW_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../init/list/nsk_init_sprites_draw.inc"

.segment "CODE"

; @brief Init stage for sprites drawing
; @warning Uses raw OAM access, requires rendering/NMI to be disabled
.export nsk_init_sprites_draw
.proc nsk_init_sprites_draw
    nsk_todo "nsk_init_sprites_draw"

    rts
.endproc

.endif