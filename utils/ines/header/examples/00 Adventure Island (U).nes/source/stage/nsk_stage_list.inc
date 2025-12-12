; @file nsk_stage_list.inc
; @brief Include module for the stages list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_LIST__
::__NSK_STAGE_LIST__ = 1

; @brief Stages of the game
.enum GAMESTAGE
    LOOK_RIGHT    ; Moving camera to the right edge
    LOOK_LEFT     ; Moving camera to the left edge
.endenum

; @brief Starting stage
GAMESTAGE_START = GAMESTAGE::LOOK_RIGHT

.ifndef ::__NSK_STAGE_LIST_ASM__
; @brief ; @brief Inits the current stage index
.import nsk_stage_init
.endif

.ifndef ::__NSK_STAGE_LIST_ASM__
; @brief Select the next stage
.import nsk_stage_next
.endif

.ifndef ::__NSK_STAGE_LIST_ASM__
; @brief Select the previous stage
.import nsk_stage_prev
.endif

.ifndef ::__NSK_STAGE_LIST_ASM__
; @brief Runs the current stage handler
.import nsk_stage_run
.endif

.endif