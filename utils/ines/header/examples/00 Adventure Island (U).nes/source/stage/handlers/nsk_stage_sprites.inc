; @file nsk_stage_sprites.inc
; @brief Include module for the sprites drawing
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_SPRITES__
::__NSK_STAGE_SPRITES__ = 1

.ifndef ::__NSK_STAGE_SPRITES_ASM__
; @brief Draw sprites
; @note In this demo no other levels are planned, so this
;    function is a common sprites drawer
.import nsk_stage_sprites_draw
; @brief Initialization routine
.import nsk_stage_sprites_draw_init
.endif

.endif