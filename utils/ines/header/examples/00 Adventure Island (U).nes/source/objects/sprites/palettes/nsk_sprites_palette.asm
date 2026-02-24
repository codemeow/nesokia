; @file nsk_sprites_palette.asm
; @brief Source module for the sprites palette
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_PALETTE_ASM__
::__NSK_SPRITES_PALETTE_ASM__ = 1

.include "nsk_sprites_palette.inc"

.segment "RODATA"

; @brief Sprites palette
.export nsk_sprites_palette
nsk_sprites_palette:
    .incbin "build/pal/sprites.pal"

.endif