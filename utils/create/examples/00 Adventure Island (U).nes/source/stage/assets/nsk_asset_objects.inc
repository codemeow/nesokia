; @file nsk_asset_objects.inc
; @brief Include module for the objects assets
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_ASSET_OBJECTS__
::__NSK_ASSET_OBJECTS__ = 1

; @page NSK simple object format
; ~~~
; xxx:
;    .byte n      ; Number of encoded spans
;    [
;       .byte l              ; Span's length in tiles
;                            ; (must be even)
;       .byte dx, dy         ; Relative X and Y of the span's beginning
;                            ; (must be even)
;       .byte a0, a1, a2 ... ; Span's data, 1st line
;       .byte b0, b1, b2 ... ; Span's data, 2nd line
;       .byte p0, p1, p3 ... ; Span's lines palettes
;                            ; (l / 2 in length, 2 bits per byte only)
;    ] × n
; ~~~
;
; ### Example
; ~~~
; ; $30 - leaves tile
; ; $40 - trunk tile
;
; ; %00 - leaves' palette
; ; %01 - trunk's palette
;
; tree:
;     .byte $04                          ; 4 spans
;
;     ; 1st span: leaves
;     .byte $06                          ; Span is 6 tiles wide
;     .byte $00, $00                     ; Span starts at (0,0)
;     .byte $30, $30, $30, $30, $30, $30 ; 1st row, all tiles are leaves
;     .byte $30, $30, $30, $30, $30, $30 ; 2nd row, all tiles are leaves
;     .byte %00, %00, %00                ; Span's palettes
;
;     ; 2nd span: leaves and trunk
;     .byte $06                          ; Span is 6 tiles wide
;     .byte $00, $02                     ; Span starts at (0,2)
;     .byte $30, $30, $40, $40, $30, $30 ; 1st row, leaves/trunk/leaves
;     .byte $30, $30, $40, $40, $30, $30 ; 2nd row, leaves/trunk/leaves
;     .byte %00, %01, %00                ; Span's palettes
;
;     ; 3rd span: leaves and trunk
;     .byte $06                          ; Span is 6 tiles wide
;     .byte $00, $04                     ; Span starts at (0,4)
;     .byte $30, $30, $40, $40, $30, $30 ; 1st row, leaves/trunk/leaves
;     .byte $30, $30, $40, $40, $30, $30 ; 2nd row, leaves/trunk/leaves
;     .byte %00, %01, %00                ; Span's palettes
;
;     ; 4th span: trunk only
;     .byte $02                          ; Span is 2 tiles wide
;     .byte $02, $06                     ; Span starts at (2, 6)
;     .byte $40, $40                     ; 1st row: trunk
;     .byte $40, $40                     ; 2nd row: trunk
;     .byte %01                          ; Span's palette
; ~~~
; As the result, the following picture should be rendered:
; ~~~
; ░░  ░░  ░░  ░░  ░░  ░░
;
; ░░  ░░  ░░  ░░  ░░  ░░
;
; ░░  ░░  ██  ██  ░░  ░░
;
; ░░  ░░  ██  ██  ░░  ░░
;
; ░░  ░░  ██  ██  ░░  ░░
;
; ░░  ░░  ██  ██  ░░  ░░
;
;         ██  ██
;
;         ██  ██
; ~~~

.ifndef ::__NSK_ASSET_OBJECTS_ASM__
; @brief Ground object
.import nsk_object_ground
.endif

.ifndef ::__NSK_ASSET_OBJECTS_ASM__
; @brief Counter object
.import nsk_object_counter
.endif

.endif