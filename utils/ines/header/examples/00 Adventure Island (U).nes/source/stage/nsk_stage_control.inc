; @file nsk_stage_control.inc
; @brief Include module for the stages control
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_CONTROL__
::__NSK_STAGE_CONTROL__ = 1

; @brief Stages of the game
.enum GAMESTAGE
    LOOK_RIGHT    ; Moving camera to the right edge
    LOOK_LEFT     ; Moving camera to the left edge
.endenum

; @brief Starting stage
GAMESTAGE_START = GAMESTAGE::LOOK_RIGHT

.ifndef ::__NSK_STAGE_CONTROL_ASM__
    ; @brief Current stage of the game
    .import game_stage_num

    ; @brief Select the next stage
    .import nsk_stage_next
    ; @brief Select the previous stage
    .import nsk_stage_prev
    ; @brief Runs the current stage handler
    .import nsk_stage_run
.endif

.endif