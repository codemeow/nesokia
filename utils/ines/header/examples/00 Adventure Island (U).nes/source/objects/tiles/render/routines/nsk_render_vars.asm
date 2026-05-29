; @file nsk_render_vars.asm
; @brief Source module for map rendering state variables
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_VARS_ASM__
::__NSK_RENDER_VARS_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_render_vars.inc"

.segment "ZEROPAGE"

; @brief Address for the selected map data
.exportzp _nsk_render_mapdata
_nsk_render_mapdata:
    .res 2

; @brief Object address
.exportzp _nsk_render_object
_nsk_render_object:
    .res 2

; @brief Collision data pointer
.exportzp _nsk_render_collisionptr
_nsk_render_collisionptr:
    .res 2

; @brief Collision map pointer
.exportzp _nsk_render_mapptr
_nsk_render_mapptr:
    .res 2

.segment "BSS"

; @brief Address for the selected nametable
.export _nsk_render_nametable
_nsk_render_nametable:
    .res 2

; @brief Attributes buffer
.export _nsk_render_attrbuf
_nsk_render_attrbuf:
    .res ::NSK::PPU::NAMETABLE::SIZE::ATTRS

; @brief Object X
.export _nsk_render_objx
_nsk_render_objx:
    .res 1

; @brief Object Y
.export _nsk_render_objy
_nsk_render_objy:
    .res 1

; @brief Width of the object
.export _nsk_render_width
_nsk_render_width:
    .res 1

; @brief Height of the object
.export _nsk_render_height
_nsk_render_height:
    .res 1

; @brief Temporary offset address counter
.export _nsk_render_offs
_nsk_render_offs:
    .res 2

; @brief Palette X quadrant part
.export _nsk_render_quadrant_posx
_nsk_render_quadrant_posx:
    .res 1
; @brief Palette Y quadrant part
.export _nsk_render_quadrant_posy
_nsk_render_quadrant_posy:
    .res 1

; @brief Palette calculations: shift
.export _nsk_render_quadrant_shift
_nsk_render_quadrant_shift:
    .res 1
; @brief Palette calculations: mask
.export _nsk_render_quadrant_mask
_nsk_render_quadrant_mask:
    .res 1
; @brief Palette calculations: value
.export _nsk_render_quadrant_value
_nsk_render_quadrant_value:
    .res 1

.endif
