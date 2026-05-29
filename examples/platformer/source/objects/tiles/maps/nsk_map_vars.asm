; @file nsk_map_vars.asm
; @brief Source module for the map variables
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_MAP_VARS_ASM__
::__NSK_MAP_VARS_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "nsk_map_vars.inc"

.segment "BSS"

; @brief Map collision blocks
.export nsk_map_grid
nsk_map_grid:
    .res ::MAP::SCREEN::PAGE * ::MAP::SCREEN::PAGES

.endif