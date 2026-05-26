; @file nsk_render_object_palettes.asm
; @brief Source module for map object palette rendering
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_OBJECT_PALETTES_ASM__
::__NSK_RENDER_OBJECT_PALETTES_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_render_object_palettes.inc"
.include "nsk_render_vars.inc"

.segment "CODE"

.assert \
    ::NSK::SCREEN::ATTR::QUADRANT::WIDTH = 2, \
    error,                                    \
    "Object palette renderer expects 2-tile-wide attribute quadrants"
.assert \
    ::NSK::SCREEN::ATTR::QUADRANT::HEIGHT = 2, \
    error,                                     \
    "Object palette renderer expects 2-tile-high attribute quadrants"
.assert \
    ::NSK::SCREEN::ATTR::BLOCK::WIDTH = 4, \
    error,                                \
    "Object palette renderer expects 4-tile-wide attribute blocks"
.assert \
    ::NSK::SCREEN::ATTR::BLOCK::HEIGHT = 4, \
    error,                                 \
    "Object palette renderer expects 4-tile-high attribute blocks"

; @brief Renders the object palettes
; @param[in] _nsk_render_objx      Object X position
; @param[in] _nsk_render_objy      Object Y position
; @param[in] _nsk_render_object    Object data address
; @param[in] _nsk_render_width     Object width
; @param[in] _nsk_render_height    Object height
; @warning Clobbers A, X, Y
.export _render_object_palettes
.proc _render_object_palettes

    lda _nsk_render_objx
    pha
    lda _nsk_render_objy
    pha
    lda _nsk_render_width
    pha
    lda _nsk_render_height
    pha

    ; The attribute table cell encodes the palettes of 16 different nametable
    ; cells:
    ;
    ; A | A | B | B
    ; --+---+---+---
    ; A | A | B | B
    ; --+---+---+---
    ; C | C | D | D
    ; --+---+---+---
    ; C | C | D | D
    ;
    ; Thus the bits in the value are used as follows:
    ;
    ; DD CC BB AA
    ; -----------
    ; 76 54 32 10
    ;
    ; 10 | 32     №0 | №1
    ; ---+--- or ----+----
    ; 54 | 76     №2 | №3
    ;
    ; Thus quadrant (№) is selected as (Y % 2) << 1 | (X % 2)
    ; Note that these are X and Y inside of the attribute table
    ; The relative to Nametable values formula is
    ; ((Y / 2) % 2) << 1 | ((X / 2) % 2)

    ; Which starting quadrants should be used
    lda _nsk_render_objy
    lsr
    and #::NSK::SCREEN::ATTR::QUADRANT::MASK
    sta _nsk_render_quadrant_posy

    lda _nsk_render_objx
    lsr
    and #::NSK::SCREEN::ATTR::QUADRANT::MASK
    sta _nsk_render_quadrant_posx

    ; Translate the nametable coordinates into attribute coordinates:
    lsr _nsk_render_height ; object height is 2 times smaller
    lsr _nsk_render_width  ; object width is 2 times smaller
    lsr _nsk_render_objy   ; object Y position is 4 times smaller
    lsr _nsk_render_objy
    lsr _nsk_render_objx   ; object X position is 4times smaller
    lsr _nsk_render_objx

    ; Then, we need to prepare the starting point of the address
    lda _nsk_render_objy
    ; Y * ::NSK::SCREEN::ATTR::BYTE::WIDTH [= Y * 8]
    asl ; x2
    asl ; x4
    asl ; x8
    ; Y * ::NSK::SCREEN::ATTR::BYTE::WIDTH + X
    clc
    adc _nsk_render_objx
    ; Store the calculated starting point
    sta _nsk_render_offs

    height:
        ; Overflow the quadrant Y
        lda _nsk_render_quadrant_posy
        and #::NSK::SCREEN::ATTR::QUADRANT::MASK
        sta _nsk_render_quadrant_posy

        ; Save the Qx position
        lda _nsk_render_quadrant_posx
        pha

        ; Save offset position, as the `width` loop will roll it ahead
        lda _nsk_render_offs
        pha

        ; Save object width
        lda _nsk_render_width
        pha

        width:
            ; Overflow the quadrant X
            lda _nsk_render_quadrant_posx
            and #::NSK::SCREEN::ATTR::QUADRANT::MASK
            sta _nsk_render_quadrant_posx

            ; The final formula is
            ;     shift   = posy << 1 | posx
            ;     mask    = ~(%11 << shift)
            ;     newbyte = (oldbyte & mask) | (quadrantvalue << shift)

            ; Calculate the quadrant shift
            lda _nsk_render_quadrant_posy
            asl
            ora _nsk_render_quadrant_posx
            sta _nsk_render_quadrant_shift

            ; Calculate the quadrant mask
            lda #::NSK::SCREEN::ATTR::PALETTE::MASK
            ldx _nsk_render_quadrant_shift
            beq :++
            :
                .repeat ::NSK::SCREEN::ATTR::PALETTE::BITS
                    asl
                .endrep
                dex
                bne :-
            :
            not
            sta _nsk_render_quadrant_mask

            ; Read the new palette value
            lda (_nsk_render_object), y
            iny
            ; Shift the value
            ldx _nsk_render_quadrant_shift
            beq :++
            :
                .repeat ::NSK::SCREEN::ATTR::PALETTE::BITS
                    asl
                .endrep
                dex
                bne :-
            :
            ; Save the value
            sta _nsk_render_quadrant_value

            ; Read the old byte
            ldx _nsk_render_offs
            lda _nsk_render_attrbuf, x
            ; Apply mask
            and _nsk_render_quadrant_mask
            ; Apply new value
            ora _nsk_render_quadrant_value

            ; Save the final palette byte into buffer
            sta _nsk_render_attrbuf, x

            ; Next offset
            lda _nsk_render_quadrant_posx
            beq :+
                inc _nsk_render_offs
            :

            ; Next X quadrant
            inc _nsk_render_quadrant_posx

            ; Width check
            dec _nsk_render_width
            bne width

        ; Restore object width
        pla
        sta _nsk_render_width

        ; Restore offset
        pla
        sta _nsk_render_offs

        pla
        sta _nsk_render_quadrant_posx

        ; Increase the offset by the attribute table width:
        lda _nsk_render_quadrant_posy
        beq :+
            lda _nsk_render_offs
            clc
            adc #::NSK::SCREEN::ATTR::BYTE::WIDTH
            sta _nsk_render_offs
        :

        ; Next Y quadrant
        inc _nsk_render_quadrant_posy

        ; Height check
        dec _nsk_render_height
        beq done
        jmp height

    done:

    pla
    sta _nsk_render_height
    pla
    sta _nsk_render_width
    pla
    sta _nsk_render_objy
    pla
    sta _nsk_render_objx

    rts
.endproc

.endif
