; @file nsk_tiles_bg.asm
; @brief Source module for the background tile objects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_BG_ASM__
::__NSK_TILES_BG_ASM__ = 1

.include "nsk_tiles_bg.inc"

.segment "RODATA"

.export nsk_object_bg_line
nsk_object_bg_line:
    .byte 4,2
    .byte $c2,$c2,$c2,$c2
    .byte $00,$00,$00,$00
    .byte 2,2

.export nsk_object_bg_up
nsk_object_bg_up:
    .byte 2,4
    .byte $00,$b0
    .byte $b2,$c0
    .byte $c0,$00
    .byte $00,$00
    .byte 2
    .byte 2

.export nsk_object_bg_down
nsk_object_bg_down:
    .byte 2,4
    .byte $b1,$00
    .byte $c1,$b3
    .byte $00,$c1
    .byte $00,$00
    .byte 2
    .byte 2

.endif
