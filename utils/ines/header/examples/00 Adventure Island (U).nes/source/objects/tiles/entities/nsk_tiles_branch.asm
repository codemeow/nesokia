; @file nsk_tiles_branch.asm
; @brief Source module for the branch tile objects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_BRANCH_ASM__
::__NSK_TILES_BRANCH_ASM__ = 1

.include "nsk_tiles_branch.inc"

.segment "RODATA"

.export nsk_object_branch_l
nsk_object_branch_l:
    .byte 8,4
    .byte $d3,$d1,$d1,$d1, $d1,$d1,$e4,$e4
    .byte $e0,$e4,$e5,$f1, $f1,$e4,$d4,$e4
    .byte $e0,$e4,$f7,$e8, $00,$f6,$e4,$e4
    .byte $f3,$f5,$00,$f8, $00,$00,$e0,$e4
    .byte 2,2,2,2
    .byte 2,2,2,2

.export nsk_object_branch_r
nsk_object_branch_r:
    .byte 8,4
    .byte $e4,$e4,$d1,$d1, $d1,$d1,$d1,$d5
    .byte $e3,$e4,$e4,$f1, $f1,$e4,$f4,$e2
    .byte $e4,$e4,$f7,$ea, $fa,$f6,$e4,$e2
    .byte $e4,$e2,$00,$00, $00,$00,$f3,$f5
    .byte 2,2,2,2
    .byte 2,2,2,2

.endif
