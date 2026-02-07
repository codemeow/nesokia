; @file nsk_stage_list.asm
; @brief Source module for the stages list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_LIST_ASM__
::__NSK_STAGE_LIST_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../stage/nsk_stage_list.inc"
.include "../stage/handlers/nsk_stage_look_left.inc"
.include "../stage/handlers/nsk_stage_look_right.inc"
.include "../stage/handlers/nsk_stage_sprites.inc"

.segment "ZEROPAGE"

; @brief Current stage of the game
game_stage_num:
    .res 1

; @brief Temporary pointer to use in `jai`
stage_jump_ptr:
    .res 2

.segment "RODATA"

; @brief Stage handlers list
game_stage_table:
    .addr nsk_stage_look_right
    .addr nsk_stage_look_left
game_stage_table_end:

; @brief Number of elements in the stage list
STAGE_TABLE_SIZE = (game_stage_table_end - game_stage_table) / 2

; @brief Stage initialization list
game_init_table:
    .addr nsk_stage_look_right_init
    .addr nsk_stage_look_left_init
    .addr nsk_stage_sprites_draw_init
game_init_table_end:

; @brief Number of elements in the init table
INIT_TABLE_SIZE = (game_init_table_end - game_init_table) / 2

.segment "CODE"

; @brief Inits the current stage index
.export nsk_stage_init
.proc nsk_stage_init
    push a

    lda #$00
    sta game_stage_num

    loop:
        jai stage_jump_ptr, game_init_table, game_stage_num
        inc game_stage_num
        lda game_stage_num
        cmp #INIT_TABLE_SIZE
        bne loop


    lda #GAMESTAGE_START
    sta game_stage_num

    pull a
    rts
.endproc

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
    jai stage_jump_ptr, game_stage_table, game_stage_num
    rts
.endproc

.endif