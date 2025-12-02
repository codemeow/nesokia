; @file nsk_render_map.inc
; @brief Include module for the simple map rendering routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_MAP__
::__NSK_RENDER_MAP__ = 1

.include "nsk_common_meta.inc"

.ifndef ::__NSK_RENDER_MAP_ASM__
; @brief Address for the selected nametable
.import _nsk_render_nametable
.endif

.ifndef ::__NSK_RENDER_MAP_ASM__
; @brief Address for the selected map data
.import _nsk_render_mapdata
.endif

.ifndef ::__NSK_RENDER_MAP_ASM__
; @brief Renders selected map on the selected nametable
.import _nsk_render_map
.endif

; @brief Renders the selected map on the selected nametable
; @param [in] nametable Nametable's address ($2000, $2400, $2800 or $2c00)
; @param [in] mapdata   NSK simple map data's address
.macro nsk_render_map nametable, mapdata
    nsk_p16_set _nsk_render_nametable, nametable, 16
    nsk_p16_set _nsk_render_mapdata,   #mapdata,   16
    jsr _nsk_render_map
.endmacro

.endif