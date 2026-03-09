; @file nsk_tiles_base.asm
; @brief Source module for the base tile objects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_BASE_ASM__
::__NSK_TILES_BASE_ASM__ = 1

.include "nsk_tiles_base.inc"

.segment "RODATA"

.export nsk_object_base_a
nsk_object_base_a:
    .byte 8, 2

    .byte $f3,$e4,$e4,$e3, $e4,$e4,$e4,$e4
    .byte $00,$f6,$e4,$e4, $e4,$e5,$e4,$e4

    .byte %10,%10,%10,%10

    .byte 1,1,1,1

.export nsk_object_base_b
nsk_object_base_b:
    .byte 8, 2

    .byte $e4,$e5,$e4,$e4, $e4,$e4,$e4,$f5
    .byte $e4,$e4,$e4,$e4, $e4,$d4,$f7,$00

    .byte %10,%10,%10,%10

    .byte 1,1,1,1

.export nsk_object_base_c
nsk_object_base_c:
    .byte 8, 2

    .byte $e4,$e4,$e3,$e4, $e4,$e4,$d4,$e4
    .byte $e4,$e4,$e4,$e5, $e4,$e4,$e4,$e4

    .byte %10,%10,%10,%10

    .byte 1,1,1,1

.endif
