; @file nsk_render_attributes.inc
; @brief Include module for map rendering attribute routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_ATTRIBUTES__
::__NSK_RENDER_ATTRIBUTES__ = 1

.ifndef ::__NSK_RENDER_ATTRIBUTES_ASM__
    ; @brief Writes the _nsk_render_attrbuf buffer to the PPU
    .import _render_attributes_write
.endif

.endif
