; @file nsk_init_sprites_draw.asm
; @brief Source module for the sprites drawing init stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_INIT_SPRITES_DRAW_ASM__
::__NSK_INIT_SPRITES_DRAW_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_init_sprites_draw.inc"

.include "../../../ppu/nsk_ppu_palette.inc"
.include "../../../objects/sprites/palettes/nsk_sprites_palette.inc"

.segment "CODE"

; @brief Init stage for sprites drawing
.export nsk_init_sprites_draw
.proc nsk_init_sprites_draw
    nsk_palette_spritesset nsk_sprites_palette

    rts
.endproc

.endif