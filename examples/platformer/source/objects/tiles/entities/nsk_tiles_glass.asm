; @file nsk_tiles_glass.asm
; @brief Source module for the glass tile objects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_GLASS_ASM__
::__NSK_TILES_GLASS_ASM__ = 1

.include "nsk_tiles_glass.inc"
.include "../utils/nsk_utils_tiles.inc"

.segment "RODATA"

.export nsk_object_glass_8
nsk_object_glass_8:
    nsk_tile_objectsize 8, 2

    .byte $a0,$a1,$a3,$a2, $a3,$a1,$a2,$a4
    .byte $00,$00,$00,$00, $00,$00,$00,$00

    .byte %00,%00,%00,%00

    .byte 1,1,1,1

.export nsk_object_glass_4
nsk_object_glass_4:
    nsk_tile_objectsize 4, 2

    .byte $a0,$a1,$a3,$a4
    .byte $00,$00,$00,$00

    .byte %00,%00

    .byte 1,1

.endif
