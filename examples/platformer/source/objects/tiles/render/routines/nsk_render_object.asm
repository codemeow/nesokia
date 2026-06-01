; @file nsk_render_object.asm
; @brief Source module for map object rendering orchestration
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_OBJECT_ASM__
::__NSK_RENDER_OBJECT_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_render_object.inc"
.include "nsk_render_object_tiles.inc"
.include "nsk_render_object_palettes.inc"
.include "nsk_render_object_collision.inc"

.segment "CODE"

; @brief Renders the object
; @param[in] _nsk_render_objx      Object X position
; @param[in] _nsk_render_objy      Object Y position
; @param[in] _nsk_render_object    Object data address
; @param[in] _nsk_render_nametable Base target nametable address
.export _render_object
.proc _render_object
    push a, x, y
    jsr _render_object_tiles
    jsr _render_object_palettes
    jsr _render_object_collision
    pull a, x, y
    rts
.endproc

.endif
