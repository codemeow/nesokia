; @file nsk_draw_vars.asm
; @brief Source module for the sprites draw variables
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_DRAW_VARS_ASM__
::__NSK_DRAW_VARS_ASM__ = 1

.include "nsk_draw_vars.inc"

.segment "BSS"

; @brief How many sprites are active in this frame
.export nsk_sprites_used
nsk_sprites_used:
    .res 1

.endif