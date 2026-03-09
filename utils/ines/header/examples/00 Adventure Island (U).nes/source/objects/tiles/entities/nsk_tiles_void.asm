; @file nsk_tiles_void.asm
; @brief Source module for the void tile object
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_VOID_ASM__
::__NSK_TILES_VOID_ASM__ = 1

.include "nsk_tiles_void.inc"

.segment "RODATA"

.export nsk_object_void
nsk_object_void:
    .byte 2,2

    .byte $00,$00
    .byte $00,$00

    .byte %10

    .byte 0

.endif
