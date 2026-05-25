; @file nsk_map_data.asm
; @brief Source module for the map
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_MAP_DATA_ASM__
::__NSK_MAP_DATA_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_map_data.inc"
.include "../nsk_tiles_list.inc"
.include "../utils/nsk_utils_maps.inc"

.segment "RODATA"

; @brief Left map data
;
; @note Considering 256×224 pixels (32 × 28 tiles) screen
; @note All coordinates must be even
.export nsk_map_left
nsk_map_left:
    .byte 34 ; Objects count

    ; Skyboxes x 12
    nsk_map_object 0, 0, nsk_object_skybox2
    nsk_map_object 8, 0, nsk_object_skybox1
    nsk_map_object 16, 0, nsk_object_skybox3
    nsk_map_object 24, 0, nsk_object_skybox0
    nsk_map_object 0, 8, nsk_object_skybox4
    nsk_map_object 8, 8, nsk_object_skybox1
    nsk_map_object 16, 8, nsk_object_skybox2
    nsk_map_object 24, 8, nsk_object_skybox1

    ; Bg
    nsk_map_object 0, 24, nsk_object_bg_line
    nsk_map_object 2, 22, nsk_object_bg_up
    nsk_map_object 4, 20, nsk_object_bg_up

    nsk_map_object 10, 14, nsk_object_bg_up
    nsk_map_object 12, 12, nsk_object_bg_up
    nsk_map_object 14, 12, nsk_object_bg_line
    nsk_map_object 18, 12, nsk_object_bg_line
    nsk_map_object 22, 12, nsk_object_bg_down
    nsk_map_object 24, 14, nsk_object_bg_down

    nsk_map_object 26, 16, nsk_object_bg_line
    nsk_map_object 30, 16, nsk_object_bg_down

    ; Floor
    nsk_map_object 0, 28, nsk_object_base_a
    nsk_map_object 8, 28, nsk_object_base_b
    nsk_map_object 24, 28, nsk_object_base_a

    ; V Tubes
    nsk_map_object 6, 12, nsk_object_tube_4v
    nsk_map_object 6, 20, nsk_object_tube_4v

    ; Platform
    nsk_map_object 20, 24, nsk_object_platform_l
    nsk_map_object 24, 24, nsk_object_tube_4h

    ; Tophat
    nsk_map_object 4, 8, nsk_object_tophat

    ; Branches
    nsk_map_object 0, 16, nsk_object_branch_l
    nsk_map_object 8, 20, nsk_object_branch_r

    ; Hills
    nsk_map_object 0, 26, nsk_object_hills_l
    nsk_map_object 8, 26, nsk_object_hills_r

    ; Glass
    nsk_map_object 14, 16, nsk_object_glass_8
    nsk_map_object 18, 20, nsk_object_glass_8

    ; Stars remover
    nsk_map_object 18, 14, nsk_object_void

; @brief Right map data
;
; @note Considering 256×224 pixels (32 × 28 tiles) screen
; @note All coordinates must be even
.export nsk_map_right
nsk_map_right:
    .byte 31 ; Objects count

    ; Skyboxes x 12
    nsk_map_object 0, 0, nsk_object_skybox0
    nsk_map_object 8, 0, nsk_object_skybox1
    nsk_map_object 16, 0, nsk_object_skybox2
    nsk_map_object 24, 0, nsk_object_skybox3
    nsk_map_object 0, 8, nsk_object_skybox4
    nsk_map_object 8, 8, nsk_object_skybox0
    nsk_map_object 16, 8, nsk_object_skybox0
    nsk_map_object 24, 8, nsk_object_skybox1

    ; Bg
    nsk_map_object 0, 18, nsk_object_bg_line
    nsk_map_object 4, 18, nsk_object_bg_line
    nsk_map_object 8, 16, nsk_object_bg_up
    nsk_map_object 10, 16, nsk_object_bg_line
    nsk_map_object 14, 16, nsk_object_bg_line
    nsk_map_object 18, 14, nsk_object_bg_up
    nsk_map_object 20, 12, nsk_object_bg_up
    nsk_map_object 22, 12, nsk_object_bg_line
    nsk_map_object 26, 12, nsk_object_bg_line
    nsk_map_object 30, 12, nsk_object_bg_down

    ; Floor
    nsk_map_object 0, 28, nsk_object_base_c
    nsk_map_object 8, 28, nsk_object_base_c
    nsk_map_object 16, 28, nsk_object_base_c
    nsk_map_object 24, 28, nsk_object_base_b


    ; Platform
    nsk_map_object 0, 24, nsk_object_tube_4h
    nsk_map_object 8, 24, nsk_object_platform_r

    ; Cave
    nsk_map_object 16, 20, nsk_object_cave_l
    nsk_map_object 28, 20, nsk_object_cave_r
    nsk_map_object 20, 20, nsk_object_cave_c
    nsk_map_object 20, 26, nsk_object_cave_b

    ; Glass
    nsk_map_object 16, 12, nsk_object_glass_4
    nsk_map_object 24, 16, nsk_object_glass_4

    ; Stars remover
    nsk_map_object 24, 14, nsk_object_void

    nsk_todo "animated small meteors in the sky"


.endif
