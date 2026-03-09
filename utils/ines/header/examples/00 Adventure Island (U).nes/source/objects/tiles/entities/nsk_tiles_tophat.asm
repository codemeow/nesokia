; @file nsk_tiles_tophat.asm
; @brief Source module for the tophat tile object
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_TOPHAT_ASM__
::__NSK_TILES_TOPHAT_ASM__ = 1

.include "nsk_tiles_tophat.inc"

.segment "RODATA"

.export nsk_object_tophat
nsk_object_tophat:
    .byte 8,4

    .byte $d3,$d1,$d1,$d1, $d1,$d1,$d1,$d5
    .byte $f3,$e4,$e4,$e3, $e4,$e4,$e4,$f5
    .byte $00,$f6,$e4,$e4, $e4,$e5,$f7,$00
    .byte $00,$00,$e0,$e4, $e4,$e2,$00,$00

    .byte %10,%10,%10,%10
    .byte %10,%10,%10,%10

    .byte 1,1,1,1
    .byte 1,1,1,1

.endif
