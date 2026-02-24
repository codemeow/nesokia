; @file nsk_stage_control.asm
; @brief Source module for the stages control
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_CONTROL_ASM__
::__NSK_STAGE_CONTROL_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../stage/nsk_stage_control.inc"
.include "../stage/handlers/nsk_stage_look_left.inc"
.include "../stage/handlers/nsk_stage_look_right.inc"
.include "../stage/handlers/nsk_stage_sprites.inc"

.segment "ZEROPAGE"

; @brief Current stage of the game
.export game_stage_num
game_stage_num:
    .res 1

; @brief Temporary pointer to use in `jai`
stage_jump_ptr:
    .res 2

.segment "RODATA"

; @brief Stage list
.scope STAGE
    TABLE:
        .addr nsk_stage_look_right
        .addr nsk_stage_look_left
    _TABLE_END:

    COUNT = (STAGE::_TABLE_END - STAGE::TABLE) / 2
.endscope

.segment "CODE"

; @brief Select the next stage
.export nsk_stage_next
.proc nsk_stage_next
    inc game_stage_num
    rts
.endproc

; @brief Select the previous stage
.export nsk_stage_prev
.proc nsk_stage_prev
    dec game_stage_num
    rts
.endproc

; @brief Runs the current stage handler
.export nsk_stage_run
.proc nsk_stage_run
    jai stage_jump_ptr, STAGE::TABLE, game_stage_num
    rts
.endproc

.endif