; @file nsk_draw_vars.inc
; @brief Include module for the sprites draw variables
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_DRAW_VARS__
::__NSK_DRAW_VARS__ = 1

.linecont +

.ifndef ::__NSK_DRAW_VARS_ASM__
    ; @brief How many sprites are active in this frame
    .import nsk_sprites_used
.endif


.endif