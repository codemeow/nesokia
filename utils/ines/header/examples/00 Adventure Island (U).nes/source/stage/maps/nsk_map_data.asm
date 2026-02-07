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
    .byte 34 ; Objects count

    ; Skyboxes x 12
    .byte 0, 0 ; X, Y
    .addr nsk_object_skybox2
    .byte 8, 0 ; X, Y
    .addr nsk_object_skybox1
    .byte 16, 0 ; X, Y
    .addr nsk_object_skybox3
    .byte 24, 0 ; X, Y
    .addr nsk_object_skybox0
    .byte 0, 8 ; X, Y
    .addr nsk_object_skybox4
    .byte 8, 8 ; X, Y
    .addr nsk_object_skybox1
    .byte 16, 8 ; X, Y
    .addr nsk_object_skybox2
    .byte 24, 8 ; X, Y
    .addr nsk_object_skybox1

    ; Floor
    .byte 0,28
    .addr nsk_object_base_a
    .byte 8,28
    .addr nsk_object_base_b
    .byte 24,28
    .addr nsk_object_base_a

    ; V Tubes
    .byte 6,12
    .addr nsk_object_tube_4v
    .byte 6,20
    .addr nsk_object_tube_4v

    ; Platform
    .byte 20,24
    .addr nsk_object_platform_l
    .byte 24,24
    .addr nsk_object_tube_4h

    ; Tophat
    .byte 4,8
    .addr nsk_object_tophat

    ; Branches
    .byte 0,16
    .addr nsk_object_branch_l
    .byte 8,20
    .addr nsk_object_branch_r

    ; Hills
    .byte 0,26
    .addr nsk_object_hills_l
    .byte 8,26
    .addr nsk_object_hills_r

    ; Glass
    .byte 14,16
    .addr nsk_object_glass_8
    .byte 18,20
    .addr nsk_object_glass_8

    ; Bg
    .byte 0,24
    .addr nsk_object_bg_line
    .byte 2,22
    .addr nsk_object_bg_up
    .byte 4,20
    .addr nsk_object_bg_up

    .byte 10,14
    .addr nsk_object_bg_up
    .byte 12,12
    .addr nsk_object_bg_up
    .byte 14,12
    .addr nsk_object_bg_line
    .byte 18,12
    .addr nsk_object_bg_line
    .byte 22,12
    .addr nsk_object_bg_down
    .byte 24,14
    .addr nsk_object_bg_down

    .byte 26,16
    .addr nsk_object_bg_line
    .byte 30,16
    .addr nsk_object_bg_down

    ; Stars remover
    .byte 18,14
    .addr nsk_object_void

; @brief Right map data
;
; @note Considering 256×224 pixels (32 × 28 tiles) screen
; @note All coordinates must be even
.export nsk_map_right
nsk_map_right:
    .byte 31 ; Objects count

    ; Skyboxes x 12
    .byte 0, 0 ; X, Y
    .addr nsk_object_skybox0
    .byte 8, 0 ; X, Y
    .addr nsk_object_skybox1
    .byte 16, 0 ; X, Y
    .addr nsk_object_skybox2
    .byte 24, 0 ; X, Y
    .addr nsk_object_skybox3
    .byte 0, 8 ; X, Y
    .addr nsk_object_skybox4
    .byte 8, 8 ; X, Y
    .addr nsk_object_skybox0
    .byte 16, 8 ; X, Y
    .addr nsk_object_skybox0
    .byte 24, 8 ; X, Y
    .addr nsk_object_skybox1

    ; Floor
    .byte 0,28
    .addr nsk_object_base_c
    .byte 8,28
    .addr nsk_object_base_c
    .byte 16,28
    .addr nsk_object_base_c
    .byte 24,28
    .addr nsk_object_base_b


    ; Platform
    .byte 0,24
    .addr nsk_object_tube_4h
    .byte 8,24
    .addr nsk_object_platform_r

    ; Cave
    .byte 16,20
    .addr nsk_object_cave_l
    .byte 28,20
    .addr nsk_object_cave_r
    .byte 20,20
    .addr nsk_object_cave_c
    .byte 20,26
    .addr nsk_object_cave_b

    ; Glass
    .byte 16,12
    .addr nsk_object_glass_4
    .byte 24,16
    .addr nsk_object_glass_4

    ; Bg
    .byte 0,18
    .addr nsk_object_bg_line
    .byte 4,18
    .addr nsk_object_bg_line
    .byte 8,16
    .addr nsk_object_bg_up
    .byte 10,16
    .addr nsk_object_bg_line
    .byte 14,16
    .addr nsk_object_bg_line
    .byte 18,14
    .addr nsk_object_bg_up
    .byte 20,12
    .addr nsk_object_bg_up
    .byte 22,12
    .addr nsk_object_bg_line
    .byte 26,12
    .addr nsk_object_bg_line
    .byte 30,12
    .addr nsk_object_bg_down

    ; Stars remover
    .byte 24,14
    .addr nsk_object_void

; Todo: animated small meteors in the sky


.endif