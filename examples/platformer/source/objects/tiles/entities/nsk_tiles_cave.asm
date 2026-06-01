; @file nsk_tiles_cave.asm
; @brief Source module for the cave tile objects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_CAVE_ASM__
::__NSK_TILES_CAVE_ASM__ = 1

.include "nsk_tiles_cave.inc"
.include "../utils/nsk_utils_tiles.inc"

.segment "RODATA"

.export nsk_object_cave_l
nsk_object_cave_l:
    nsk_tile_objectsize 4, 8

    .byte $00,$d0,$d1,$d1
    .byte $d3,$d4,$e4,$f1
    .byte $e0,$e4,$f7,$ea
    .byte $e0,$e2,$00,$00

    .byte $f3,$e4,$d2,$00
    .byte $00,$e0,$e4,$d2
    .byte $00,$e0,$f4,$e2
    .byte $e6,$e4,$e4,$e2

    .byte %10,%10
    .byte %10,%10
    .byte %10,%10
    .byte %10,%10

    .byte 1,1
    .byte 1,1
    .byte 1,1
    .byte 1,1

.export nsk_object_cave_r
nsk_object_cave_r:
    nsk_tile_objectsize 4, 8

    .byte $d1,$d1,$d1,$d5
    .byte $f1,$e4,$e3,$e2
    .byte $00,$f6,$e4,$e2
    .byte $00,$00,$e0,$e2

    .byte $00,$e6,$e4,$f5
    .byte $d3,$e4,$f7,$00
    .byte $e0,$e4,$e7,$00
    .byte $e0,$e4,$e4,$d5

    .byte %10,%10
    .byte %10,%10
    .byte %10,%10
    .byte %10,%10

    .byte 1,1
    .byte 1,1
    .byte 1,1
    .byte 1,1

.export nsk_object_cave_c
nsk_object_cave_c:
    nsk_tile_objectsize 8, 4

    .byte $d1,$d1,$d1,$d1, $d1,$d1,$d1,$d1
    .byte $f1,$f1,$db,$dc, $dd,$de,$f1,$f1
    .byte $00,$e8,$f0,$f1, $f1,$f5,$e9,$fa
    .byte $00,$f8,$00,$00, $00,$00,$f9,$00

    .byte %10,%10,%10,%10
    .byte %10,%10,%10,%10

    .byte 1,1,1,1
    .byte 1,1,1,1

.export nsk_object_cave_b
nsk_object_cave_b:
    nsk_tile_objectsize 8, 2

    .byte $d1,$d1,$d1,$d1, $d1,$d1,$d1,$d1
    .byte $20,$20,$20,$20, $20,$20,$20,$20

    .byte %00,%00,%00,%00

    .byte 1,1,1,1

.endif
