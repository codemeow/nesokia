; @file nsk_map_left.asm
; @brief Source module for the left map
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_MAP_LEFT_ASM__
::__NSK_MAP_LEFT_ASM__ = 1

.include "../../stage/maps/nsk_map_left.inc"
.include "../../stage/assets/nsk_asset_objects.inc"

.segment "RODATA"

; @brief Left map data
; @note The Y scroll should be set to shift the top row down by 8 pixels.
; This places the overscan area at the bottom of the nametable and
; simplifies attribute table handling.
;
; @note Considering 256×224 pixels (32 × 28 tiles) screen, according to the
; above note
.export nsk_map_left
nsk_map_left:
    .byte $09 ; 9objects

    ; Test counter
        .byte $00, $00 ; At 0,0
        .addr nsk_object_counter
    ; Grounds
        .byte $00, $1a ; At 0,26
        .addr nsk_object_ground
        .byte $04, $1a ; At 4,26
        .addr nsk_object_ground
        .byte $08, $1a ; At 8,26
        .addr nsk_object_ground
        .byte $0c, $18 ; At 12,24
        .addr nsk_object_ground
        .byte $10, $18 ; At 16,24
        .addr nsk_object_ground
        .byte $14, $16 ; At 20,22
        .addr nsk_object_ground
        .byte $18, $14 ; At 24,20
        .addr nsk_object_ground
        .byte $1c, $12 ; At 28,18
        .addr nsk_object_ground

.endif