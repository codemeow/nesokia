; @file nsk_stage_list.asm
; @brief Source module for the stages list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_LIST_ASM__
::__NSK_STAGE_LIST_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../stage/nsk_stage_list.inc"
.include "../stage/handlers/nsk_stage_ppu_disable.inc"
.include "../stage/handlers/nsk_stage_tiles_draw.inc"
.include "../stage/handlers/nsk_stage_sprites_draw.inc"
.include "../stage/handlers/nsk_stage_ppu_enable.inc"
.include "../stage/handlers/nsk_stage_game_loop.inc"

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
    .addr nsk_stage_ppu_disable
    .addr nsk_stage_tiles_draw
    .addr nsk_stage_sprites_draw
    .addr nsk_stage_ppu_enable
    .addr nsk_stage_game_loop

.segment "CODE"

; @brief Inits the current stage index
.export nsk_stage_init
.proc nsk_stage_init
    lda #STAGE::PPU_DISABLE
    sta game_stage_num
    rts
.endproc

; @brief Select the next stage
.export nsk_stage_next
.proc nsk_stage_next
    inc game_stage_num
    rts
.endproc

; @brief Runs the current stage handler
.export nsk_stage_run
.proc nsk_stage_run
    jai stage_jump_ptr, game_stage_table, game_stage_num
    rts
.endproc

.endif