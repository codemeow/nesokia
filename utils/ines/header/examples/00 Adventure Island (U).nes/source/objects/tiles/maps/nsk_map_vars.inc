; @file nsk_map_vars.inc
; @brief Include module for the map variables
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_MAP_VARS__
::__NSK_MAP_VARS__ = 1

.include "nsk_common_meta.inc"

; @brief Map grid types
.scope MAP
    ; @brief Collision types
    .scope COLLISION
        EMPTY = 0  ; @< Empty block
        SOLID = 1  ; @< Solid block
    .endscope

    ; @brief Metablocks screen data
    .scope SCREEN
        ; @brief Width of the screen in metablocks
        WIDTH  = ::NSK::SCREEN::TILEMAP::WIDTH   / 2
        ; @brief Height of the screen in metablocks
        HEIGHT = ::NSK::SCREEN::TILEMAP::HEIGHT  / 2

        ; @ brief Size of one screen in metablocks
        PAGE   = ::MAP::SCREEN::WIDTH * ::MAP::SCREEN::HEIGHT
        ; @Brief Number of screen used
        PAGES  = 2
    .endscope
.endscope

.ifndef ::__NSK_MAP_VARS_ASM__
    ; @brief Map collision blocks
    .import nsk_map_grid
.endif

.endif
