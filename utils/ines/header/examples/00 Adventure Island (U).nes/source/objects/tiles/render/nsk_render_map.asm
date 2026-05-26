; @file nsk_render_map.asm
; @brief Source module for the simple map rendering routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_MAP_ASM__
::__NSK_RENDER_MAP_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_render_map.inc"
.include "../maps/nsk_map_vars.inc"

nsk_todo "Use common constant"

.include "routines/nsk_render_vars.inc"
.include "routines/nsk_render_clear.inc"
.include "routines/nsk_render_attributes.inc"
.include "routines/nsk_render_object.inc"

.segment "CODE"

; @brief Renders the map
; @param[in] _nsk_render_mapdata   Address of the map to render
; @param[in] _nsk_render_nametable Target nametable address
.proc _render_map
    push a, x, y

    ldy #$00
    lda (_nsk_render_mapdata), y
    beq done

    tax
    iny

    loop:
        lda (_nsk_render_mapdata), y
        sta _nsk_render_objx
        iny
        lda (_nsk_render_mapdata), y
        sta _nsk_render_objy
        iny

        lda (_nsk_render_mapdata), y
        sta _nsk_render_object
        iny
        lda (_nsk_render_mapdata), y
        sta _nsk_render_object + 1
        iny

        jsr _render_object

        dex
        bne loop

    done:

    pull a, x, y
    rts
.endproc

; @brief Renders selected map on the selected nametable
; @note Requires the PPUCTRL flag VRAM_INC to be set to VRAM_INC_1
.export _nsk_render_map
.proc _nsk_render_map
    jsr _render_attrbuf_clear
    jsr _render_nametable_clear

    jsr _render_map

    jsr _render_attributes_write
    rts
.endproc

.endif
