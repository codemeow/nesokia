; @file nsk_tiles_tube.asm
; @brief Source module for the tube tile objects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_TUBE_ASM__
::__NSK_TILES_TUBE_ASM__ = 1

.include "nsk_tiles_tube.inc"

.segment "RODATA"

.export nsk_object_tube_4v
nsk_object_tube_4v:
    .byte 4,8
    .byte $e0,$f4,$e4,$e2
    .byte $e0,$e4,$e4,$e2
    .byte $e0,$e4,$d4,$e2
    .byte $e0,$e4,$e4,$e2

    .byte $e0,$e4,$e4,$e2
    .byte $e0,$e3,$e4,$e2
    .byte $e0,$e4,$e4,$e2
    .byte $e0,$e4,$e4,$e2
    .byte 2,2
    .byte 2,2
    .byte 2,2
    .byte 2,2

.export nsk_object_tube_4h
nsk_object_tube_4h:
    .byte 8,4
    .byte $d1,$d1,$d1,$d1, $d1,$d1,$d1,$d1
    .byte $e4,$e3,$e4,$e4, $e4,$e4,$e4,$e4
    .byte $e4,$e4,$e3,$e4, $e4,$e4,$f4,$e4
    .byte $e4,$e4,$e4,$e4, $e5,$e4,$e4,$e4
    .byte 2,2,2,2
    .byte 2,2,2,2

.endif
