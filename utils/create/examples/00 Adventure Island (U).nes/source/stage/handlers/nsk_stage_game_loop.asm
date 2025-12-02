; @file nsk_stage_game_loop.asm
; @brief Source module for the game loop stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_GAME_LOOP_ASM__
::__NSK_STAGE_GAME_LOOP_ASM__ = 1

.include "nsk_common_meta.inc"

.segment "CODE"

; @brief Stage for game loop
.export nsk_stage_game_loop
.proc nsk_stage_game_loop
    nsk_todo "nsk_stage_game_loop"

    rts
.endproc

.endif