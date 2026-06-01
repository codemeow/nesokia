; @file nsk_utils_maps.inc
; @brief Include module for tile map declaration helpers
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_UTILS_MAPS__
::__NSK_UTILS_MAPS__ = 1

.include "nsk_common_meta.inc"

; @brief Declares a map object entry with assembly-time format checks
;
; @param[in] x      Object X position in tiles
; @param[in] y      Object Y position in tiles
; @param[in] addr   Tile object data address
.macro nsk_map_object posx, posy, addr
    .assert posx >= 0, error, "Map object X should be greater than or equal to 0"
    .assert posy >= 0, error, "Map object Y should be greater than or equal to 0"
    .assert \
        posx <= ::NSK::SCREEN::TILEMAP::WIDTH, \
        error,                              \
        "Map object X should fit the screen"
    .assert \
        posy <= ::NSK::SCREEN::TILEMAP::HEIGHT, \
        error,                               \
        "Map object Y should fit the screen"
    .assert (posx .mod 2) = 0, error, "Map object X should be even"
    .assert (posy .mod 2) = 0, error, "Map object Y should be even"

    .byte posx, posy
    .addr addr
.endmacro

.endif
