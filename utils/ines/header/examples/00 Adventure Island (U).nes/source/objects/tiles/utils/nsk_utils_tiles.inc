; @file nsk_utils_tiles.inc
; @brief Include module for tile object declaration helpers
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_UTILS_TILES__
::__NSK_UTILS_TILES__ = 1

.include "nsk_common_meta.inc"

; @brief Declares a tile object size with assembly-time format checks
;
; @param[in] width  Object width in tiles
; @param[in] height Object height in tiles
.macro nsk_tile_objectsize width, height
    .assert width > 0, error, "Tile object width should be greater than 0"
    .assert height > 0, error, "Tile object height should be greater than 0"
    .assert (width .mod 2) = 0, error, "Tile object width should be even"
    .assert (height .mod 2) = 0, error, "Tile object height should be even"
    .assert \
        width <= ::NSK::SCREEN::TILEMAP::WIDTH, \
        error,                                  \
        "Tile object width should fit the screen"
    .assert \
        height <= ::NSK::SCREEN::TILEMAP::HEIGHT, \
        error,                                    \
        "Tile object height should fit the screen"

    .byte width, height
.endmacro

.endif
