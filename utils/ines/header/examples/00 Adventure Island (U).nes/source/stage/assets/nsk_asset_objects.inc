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
; @brief Ground object
.import nsk_object_ground
.endif

.ifndef ::__NSK_ASSET_OBJECTS_ASM__
; @brief Counter object
.import nsk_object_counter
.endif

.ifndef ::__NSK_ASSET_OBJECTS_ASM__
; @brief Tree object
.import nsk_object_tree
.endif

.endif