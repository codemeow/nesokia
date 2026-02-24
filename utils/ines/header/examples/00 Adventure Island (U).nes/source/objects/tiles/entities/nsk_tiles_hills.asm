; @file nsk_tiles_hills.asm
; @brief Source module for the hills tile objects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_HILLS_ASM__
::__NSK_TILES_HILLS_ASM__ = 1

.include "nsk_tiles_hills.inc"

.segment "RODATA"

.export nsk_object_hills_l
nsk_object_hills_l:
    .byte 8,2
    .byte $d3,$d1,$d1,$d2, $00,$e6,$e4,$e4
    .byte $e0,$e4,$e4,$e4, $d1,$e4,$d4,$e4
    .byte 2,2,2,2

.export nsk_object_hills_r
nsk_object_hills_r:
    .byte 8,2
    .byte $e4,$e4,$e7,$d3, $d1,$d1,$d1,$d5
    .byte $e4,$e4,$e4,$e4, $e3,$e4,$e4,$e2
    .byte 2,2,2,2

.endif
