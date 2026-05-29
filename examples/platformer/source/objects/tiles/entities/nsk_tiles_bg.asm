; @file nsk_tiles_bg.asm
; @brief Source module for the background tile objects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_BG_ASM__
::__NSK_TILES_BG_ASM__ = 1

.include "nsk_tiles_bg.inc"
.include "../utils/nsk_utils_tiles.inc"

.segment "RODATA"

.export nsk_object_bg_line
nsk_object_bg_line:
    nsk_tile_objectsize 4, 2

    .byte $c2,$c2,$c2,$c2
    .byte $00,$00,$00,$00

    .byte %10,%10

    .byte 0,0

.export nsk_object_bg_up
nsk_object_bg_up:
    nsk_tile_objectsize 2, 4

    .byte $00,$b0
    .byte $b2,$c0
    .byte $c0,$00
    .byte $00,$00

    .byte %10
    .byte %10

    .byte 0
    .byte 0

.export nsk_object_bg_down
nsk_object_bg_down:
    nsk_tile_objectsize 2, 4

    .byte $b1,$00
    .byte $c1,$b3
    .byte $00,$c1
    .byte $00,$00

    .byte %10
    .byte %10

    .byte 0
    .byte 0

.endif
