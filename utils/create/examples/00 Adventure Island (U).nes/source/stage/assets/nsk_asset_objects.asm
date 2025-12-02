; @file nsk_asset_objects.asm
; @brief Source module for the objects assets
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_ASSET_OBJECTS_ASM__
::__NSK_ASSET_OBJECTS_ASM__ = 1

.include "../../stage/assets/nsk_asset_objects.inc"

.segment "RODATA"

; @brief Ground object
; Ground surface, 2 2×2 tiles
.export nsk_object_ground
nsk_object_ground:
    .byte $01                   ; One span only
    .byte $04                   ; 4 tiles wide
    .byte $00, $00              ; No span shift
    .byte $e0, $e1, $e0, $e1    ; 1st row
    .byte $f0, $f1, $f0, $f1    ; 2nd row
    .byte %00, %00              ; Palettes

; @brief Counter object
; Test counter, 1 2×2 tiles
.export nsk_object_counter
nsk_object_counter:
    .byte $01                   ; One span only
    .byte $02                   ; 2 tiles wide
    .byte $00, $00              ; No span shift
    .byte $00, $01              ; 1st row
    .byte $00, $00              ; 2nd row
    .byte %11                   ; Palette

.endif