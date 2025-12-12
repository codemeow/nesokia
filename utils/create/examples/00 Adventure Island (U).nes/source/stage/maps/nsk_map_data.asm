; @file nsk_map_data.asm
; @brief Source module for the map
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_MAP_DATA_ASM__
::__NSK_MAP_DATA_ASM__ = 1

.include "../../stage/maps/nsk_map_data.inc"
.include "../../stage/assets/nsk_asset_objects.inc"

.segment "RODATA"

; @brief Left map data
;
; @note Considering 256×224 pixels (32 × 28 tiles) screen
; @note All coordinates must be even
.export nsk_map_left
nsk_map_left:
    .byte 10 ; 9 objects

    ; Test counter
    .byte 4, 4
    .addr nsk_object_counter

    ; Grounds
    .byte  0, 26
    .addr nsk_object_ground
    .byte  4, 26
    .addr nsk_object_ground
    .byte  8, 26
    .addr nsk_object_ground
    .byte 12, 24
    .addr nsk_object_ground
    .byte 16, 24
    .addr nsk_object_ground
    .byte 20, 22
    .addr nsk_object_ground
    .byte 24, 20
    .addr nsk_object_ground
    .byte 28, 18
    .addr nsk_object_ground

    ; Tree
    .byte 12, 22
    .addr nsk_object_tree


; @brief Right map data
;
; @note Considering 256×224 pixels (32 × 28 tiles) screen
; @note All coordinates must be even
.export nsk_map_right
nsk_map_right:
    .byte 12 ; 10 objects

    ; Grounds
    .byte 0, 18
    .addr nsk_object_ground
    .byte 4, 18
    .addr nsk_object_ground
    .byte 8, 18
    .addr nsk_object_ground
    .byte 12, 18
    .addr nsk_object_ground
    .byte 16, 20
    .addr nsk_object_ground
    .byte 20, 22
    .addr nsk_object_ground
    .byte 24, 24
    .addr nsk_object_ground
    .byte 28, 26
    .addr nsk_object_ground

    ; Counter
    .byte 24, 4
    .addr nsk_object_counter

    ; Tree
    .byte 26, 22
    .addr nsk_object_tree
    .byte 0, 16
    .addr nsk_object_tree
    .byte 2, 16
    .addr nsk_object_tree

.endif