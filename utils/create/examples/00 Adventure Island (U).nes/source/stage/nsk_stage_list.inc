; @file nsk_stage_list.inc
; @brief Include module for the stages list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_LIST__
::__NSK_STAGE_LIST__ = 1

; @brief Stages of the game
.enum STAGE
    PPU_DISABLE  ; Disable PPU to prepare mass rendering
    TILES_DRAW   ; Render whole field
    SPRITES_DRAW ; Render all sprites
    PPU_ENABLE   ; Enable PPU back
    GAME_LOOP    ; Game/animation loop
.endenum

.ifndef ::__NSK_STAGE_LIST_ASM__
; @brief ; @brief Inits the current stage index
.import nsk_stage_init
.endif

.ifndef ::__NSK_STAGE_LIST_ASM__
; @brief Select the next stage
.import nsk_stage_next
.endif

.ifndef ::__NSK_STAGE_LIST_ASM__
; @brief Runs the current stage handler
.import nsk_stage_run
.endif

.endif