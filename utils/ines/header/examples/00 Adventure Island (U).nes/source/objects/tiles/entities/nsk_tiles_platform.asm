; @file nsk_tiles_platform.asm
; @brief Source module for the platform tile objects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_PLATFORM_ASM__
::__NSK_TILES_PLATFORM_ASM__ = 1

.include "nsk_tiles_platform.inc"

.segment "RODATA"

.export nsk_object_platform_l
nsk_object_platform_l:
    .byte 4,4
    .byte $d3,$d1,$d1,$d1
    .byte $e0,$e3,$e4,$e5
    .byte $f3,$f1,$f1,$e4
    .byte $00,$00,$00,$f6
    .byte 2,2
    .byte 2,2

.export nsk_object_platform_r
nsk_object_platform_r:
    .byte 8,4
    .byte $d1,$d1,$d1,$d1, $d5,$00,$00,$00
    .byte $e4,$e4,$e4,$e4, $e4,$e7,$00,$00
    .byte $e4,$d4,$e1,$e4, $e4,$e4,$d5,$00
    .byte $e4,$e4,$e4,$e4, $e4,$f4,$e4,$e7
    .byte 2,2,2,2
    .byte 2,2,2,2

.endif
