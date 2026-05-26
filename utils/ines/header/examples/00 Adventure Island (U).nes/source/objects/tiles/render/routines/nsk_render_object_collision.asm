; @file nsk_render_object_collision.asm
; @brief Source module for map object collision rendering
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_OBJECT_COLLISION_ASM__
::__NSK_RENDER_OBJECT_COLLISION_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_render_object_collision.inc"
.include "nsk_render_vars.inc"
.include "../../maps/nsk_map_vars.inc"

.segment "CODE"

; @brief Assign the object collision to the global map
;
; @param[in] Y                     Pointer position in the object
; @param[in] _nsk_render_objx      Object X position
; @param[in] _nsk_render_objy      Object Y position
; @param[in] _nsk_render_object    Object data address
; @param[in] _nsk_render_nametable Base target nametable address
; @param[in] _nsk_render_width     Object width
; @param[in] _nsk_render_height    Object height
.export _render_object_collision
.proc _render_object_collision
    push a, x, y

    ; --- Preserve object size ---
    lda _nsk_render_width
    pha
    lda _nsk_render_height
    pha

    ; --- Convert to metablocks (2x2 tiles) ---
    lda _nsk_render_width
    lsr
    sta _nsk_render_width
    lda _nsk_render_height
    lsr
    sta _nsk_render_height

    ; --- Base offset ---
    lda _nsk_render_objy
    lsr
    sta _nsk_render_quadrant_posy
    lda _nsk_render_objx
    lsr
    sta _nsk_render_quadrant_posx

    lda _nsk_render_quadrant_posy
    asl
    asl
    asl
    asl
    clc
    adc _nsk_render_quadrant_posx
    sta _nsk_render_offs
    lda #$00
    sta _nsk_render_offs + 1

    nsk_todo "Need something smarter here"
    ; --- Page select ---
    lda _nsk_render_nametable + 1
    cmp #>NSK::PPU::NAMETABLE::TABLE1
    bne :+
        lda _nsk_render_offs
        clc
        adc #::MAP::SCREEN::PAGE
        sta _nsk_render_offs
        bcc :+
            inc _nsk_render_offs + 1
    :

    ; --- Pointers ---
    lda #<nsk_map_grid
    clc
    adc _nsk_render_offs
    sta _nsk_render_mapptr
    lda #>nsk_map_grid
    adc _nsk_render_offs + 1
    sta _nsk_render_mapptr + 1

    tya
    clc
    adc _nsk_render_object
    sta _nsk_render_collisionptr
    lda _nsk_render_object + 1
    adc #0
    sta _nsk_render_collisionptr + 1

    ; --- Copy rows ---
    lda _nsk_render_height
    sta _nsk_render_quadrant_mask
    beq done

    row:
        ldy #$00
        col:
            lda (_nsk_render_collisionptr), y
            sta (_nsk_render_mapptr), y
            iny
            cpy _nsk_render_width
            bne col

        lda _nsk_render_collisionptr
        clc
        adc _nsk_render_width
        sta _nsk_render_collisionptr
        lda _nsk_render_collisionptr + 1
        adc #0
        sta _nsk_render_collisionptr + 1

        lda _nsk_render_mapptr
        clc
        adc #::MAP::SCREEN::WIDTH
        sta _nsk_render_mapptr
        bcc :+
            inc _nsk_render_mapptr + 1
        :

        dec _nsk_render_quadrant_mask
        bne row

    done:
    ; --- Restore object size ---
    pla
    sta _nsk_render_height
    pla
    sta _nsk_render_width

    pull a, x, y
    rts
.endproc

.endif
