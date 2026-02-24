; @file nsk_tiles_list.inc
; @brief Include module for the tile objects list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_TILES_LIST__
::__NSK_TILES_LIST__ = 1

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
;     .byte $04, $02              ; (w,h) = (4x2)
;     .byte $e0, $e1, $e0, $e1    ; Data
;     .byte $f0, $f1, $f0, $f1    ; Data
;     .byte %00, %00              ; Palettes (pw,ph) = (w/2,h/2)
; ~~~

.include "entities/nsk_tiles_base.inc"
.include "entities/nsk_tiles_bg.inc"
.include "entities/nsk_tiles_branch.inc"
.include "entities/nsk_tiles_cave.inc"
.include "entities/nsk_tiles_glass.inc"
.include "entities/nsk_tiles_hills.inc"
.include "entities/nsk_tiles_platform.inc"
.include "entities/nsk_tiles_skybox.inc"
.include "entities/nsk_tiles_tophat.inc"
.include "entities/nsk_tiles_tube.inc"
.include "entities/nsk_tiles_void.inc"

.endif
