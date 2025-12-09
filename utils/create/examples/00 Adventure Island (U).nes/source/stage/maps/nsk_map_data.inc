; @file nsk_map_data.inc
; @brief Include module for the map
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_MAP_DATA__
::__NSK_MAP_DATA__ = 1

; @page NSK simple map format
; ~~~
; xxx:
;    .byte n                    ; Objects count
;    [
;       .byte x, y              ; Object position (must be even)
;       .addr object            ; NSK simple object address
;    ] × n

.ifndef ::__NSK_MAP_DATA_ASM__
; @brief Left map data
.import nsk_map_left
.endif

.ifndef ::__NSK_MAP_DATA_ASM__
; @brief Right map data
.import nsk_map_right
.endif

.endif