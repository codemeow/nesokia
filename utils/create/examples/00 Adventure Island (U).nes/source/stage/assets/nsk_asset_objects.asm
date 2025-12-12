; @file nsk_asset_objects.asm
; @brief Source module for the objects assets
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_ASSET_OBJECTS_ASM__
::__NSK_ASSET_OBJECTS_ASM__ = 1

.include "../../stage/assets/nsk_asset_objects.inc"

.segment "RODATA"

; @brief Ground object
; Ground surface
.export nsk_object_ground
nsk_object_ground:
    .byte $04, $02              ; (w,h)= (4x2)
    .byte $e0, $e1, $e0, $e1    ; Data
    .byte $f0, $f1, $f0, $f1    ; Data
    .byte %00, %00              ; Palettes (pw,ph) = (w/2,h/2)

; @brief Counter object
; Test counter
.export nsk_object_counter
nsk_object_counter:
    .byte $02, $02              ; (w,h) = (2x2)
    .byte $01, $02              ; Data
    .byte $03, $04              ; Data
    .byte %11                   ; Palette

; @brief Tree object
; Simple tree
.export nsk_object_tree
nsk_object_tree:
    .byte $02, $02              ; (w,h) = (2x2)
    .byte $c0, $c1              ; Data
    .byte $d0, $d1              ; Data
    .byte %01                   ; Palette


.endif