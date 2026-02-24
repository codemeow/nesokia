; @file nsk_tiles_palette.asm
; @brief Source module for the tiles palette
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_PALETTE_ASM__
::__NSK_TILES_PALETTE_ASM__ = 1

.include "nsk_tiles_palette.inc"

.segment "RODATA"

; @brief Tiles palette
.export nsk_tiles_palette
nsk_tiles_palette:
    .incbin "build/pal/back.pal"

.endif