; @file nsk_asset_objects.inc
; @brief Include module for the objects assets
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_ASSET_OBJECTS__
::__NSK_ASSET_OBJECTS__ = 1

; @page NSK simple object format
; ~~~
; xxx:
;    .byte w, h ; Width and height of the object
;               ; Must be even
;    [
;        [
;            .byte x ; Tile
;        ] × w
;    ] × h
;    [
;        [
;            .byte p ; Palette data (%00..%11)
;        ] × w / 2
;    ] × h / 2
; ~~~
;
; ### Example
; ~~~
; nsk_object_ground:
;     .byte $04, $02              ; (w,h)= (4x2)
;     .byte $e0, $e1, $e0, $e1    ; Data
;     .byte $f0, $f1, $f0, $f1    ; Data
;     .byte %00, %00              ; Palettes (pw,ph) = (w/2,h/2)
; ~~~


.ifndef ::__NSK_ASSET_OBJECTS_ASM__
.import nsk_object_skybox0
.import nsk_object_skybox1
.import nsk_object_skybox2
.import nsk_object_skybox3
.import nsk_object_skybox4

.import nsk_object_base_a
.import nsk_object_base_b
.import nsk_object_base_c

.import nsk_object_tube_4v
.import nsk_object_tube_4h

.import nsk_object_tophat
.import nsk_object_branch_l
.import nsk_object_branch_r

.import nsk_object_hills_l
.import nsk_object_hills_r

.import nsk_object_platform_l
.import nsk_object_platform_r

.import nsk_object_cave_l
.import nsk_object_cave_r
.import nsk_object_cave_c
.import nsk_object_cave_b

.import nsk_object_glass_8
.import nsk_object_glass_4

.import nsk_object_bg_line
.import nsk_object_bg_up
.import nsk_object_bg_down

.import nsk_object_void

.endif

.endif