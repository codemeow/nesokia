; @file nsk_init_tiles_draw.inc
; @brief Include module for the tiles drawing init stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_INIT_TILES_DRAW__
::__NSK_INIT_TILES_DRAW__ = 1

.ifndef ::__NSK_INIT_TILES_DRAW_ASM__
; @brief Init stage for tiles drawing
; @warning Uses raw OAM access, requires rendering/NMI to be disabled
.import nsk_init_tiles_draw
.endif

.endif