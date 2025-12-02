; @file nsk_map_left.inc
; @brief Include module for the left map
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_MAP_LEFT__
::__NSK_MAP_LEFT__ = 1

; @page NSK simple map format
; ~~~
; xxx:
;    .byte n                    ; Objects count
;    [
;       .byte x, y              ; Object position (must be even)
;       .addr object            ; NSK simple object address
;    ] × n

.ifndef ::__NSK_MAP_LEFT_ASM__
; @brief Left map data
.import nsk_map_left
.endif

.endif