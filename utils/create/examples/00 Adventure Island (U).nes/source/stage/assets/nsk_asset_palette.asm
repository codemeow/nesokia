; @file nsk_asset_palette.asm
; @brief Source module for the palettes list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_ASSET_PALETTE_ASM__
::__NSK_ASSET_PALETTE_ASM__ = 1

.include "../../stage/assets/nsk_asset_palette.inc"

.segment "RODATA"

; @brief Background palette
.export nsk_palette_back
nsk_palette_back:
    .incbin "../../../chrrom/pal/pal-0.pal"


; @brief Sprites palette
.export nsk_palette_sprites
nsk_palette_sprites:
    .incbin "../../../chrrom/pal/pal-1.pal"

.endif