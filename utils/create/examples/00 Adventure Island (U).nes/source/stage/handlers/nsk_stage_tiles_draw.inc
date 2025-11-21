; @file nsk_stage_tiles_draw.inc
; @brief Include module for the tiles drawing stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_TILES_DRAW__
::__NSK_STAGE_TILES_DRAW__ = 1

.ifndef ::__NSK_STAGE_TILES_DRAW_ASM__
; @brief Stage for tiles drawing
; @warning Uses raw OAM access, requires rendering/NMI to be disabled
.import nsk_stage_tiles_draw
.endif

.endif