; @file nsk_render_map.inc
; @brief Include module for the simple map rendering routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_MAP_ASM__
::__NSK_RENDER_MAP_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../stage/render/nsk_render_map.inc"

; @brief Size of the attributes table of one nametable
::NSK_ATTRTABLE_SIZE = 64
nsk_todo "Use common constant"

.segment "ZEROPAGE"

; @brief Address for the selected map data
.exportzp _nsk_render_mapdata
_nsk_render_mapdata:
    .res 2

; @brief Object address
_nsk_render_object:
    .res 2

.segment "BSS"

; @brief Address for the selected nametable
.export _nsk_render_nametable
_nsk_render_nametable:
    .res 2

; @brief Attributes buffer
_nsk_render_attrbuf:
    .res NSK_ATTRTABLE_SIZE

; @brief Object X
_nsk_render_objx:
    .res 1

; @brief Object Y
_nsk_render_objy:
    .res 1

; @brief Width of the object
_nsk_render_width:
    .res 1

; @brief Height of the object
_nsk_render_height:
    .res 1

; @brief Temporary offset address counter
_nsk_render_offs:
    .res 2

; @brief Palette X quadrant part
_nsk_render_quadrant_posx:
    .res 1
; @brief Palette Y quadrant part
_nsk_render_quadrant_posy:
    .res 1

; @brief Palette calculations: shift
_nsk_render_quadrant_shift:
    .res 1
; @brief Palette calculations: mask
_nsk_render_quadrant_mask:
    .res 1
; @brief Palette calculations: value
_nsk_render_quadrant_value:
    .res 1

.segment "CODE"

; @brief Clears the nametables' attributes buffer
.proc _render_attrbuf_clear
    push a, x

    lda #$00
    ldx #::NSK_ATTRTABLE_SIZE

    loop:
        sta _nsk_render_attrbuf - 1, x
        dex
        bne loop

    pull a, x
    rts
.endproc

; @brief Clears the nametables' tiles
; Sets the whole tile background to $00 tile
.proc _render_nametable_clear
    push a, x, y

    nsk_ppu_addrset _nsk_render_nametable

    lda #$00

    ldy #::NSK::SCREEN::TILEMAP::HEIGHT
    rows:
        ldx #::NSK::SCREEN::TILEMAP::WIDTH

        cols:
            sta ::NSK::CPU::PPU::PPUDATA
            dex
            bne cols

        dey
        bne rows

    pull a, x, y
    rts
.endproc

; @brief Clears the nametables' attributes
.proc _render_attributes_clear
    push a, x

    nsk_ppu_addrset_attrtable _nsk_render_nametable

    lda #$ff
    nsk_debug "Attributes filler: lda #$00"
    ldx #::NSK::PPU::NAMETABLE::SIZE::ATTRS
    loop:
        sta ::NSK::CPU::PPU::PPUDATA
        dex
        bne loop

    pull a, x
    rts
.endproc

; @brief Writes the _nsk_render_attrbuf buffer to the PPU
.proc _render_attributes_write
    push a, x

    nsk_ppu_addrset_attrtable _nsk_render_nametable

    ldx #$00

    loop:
        lda _nsk_render_attrbuf, x
        sta ::NSK::CPU::PPU::PPUDATA
        inx
        cpx #::NSK::PPU::NAMETABLE::SIZE::ATTRS
        bne loop

    pull a, x
    rts
.endproc

; @brief Renders the object tiles
; @param[in] _nsk_render_objx      Object X position
; @param[in] _nsk_render_objy      Object Y position
; @param[in] _nsk_render_object    Object data address
; @param[in] _nsk_render_nametable Base target nametable address
; @warning Clobbers A, X, Y
.proc _render_object_tiles

    ldy #$00

    lda _nsk_render_objy
    pha

    lda (_nsk_render_object), y
    iny
    sta _nsk_render_width
    lda (_nsk_render_object), y
    iny
    sta _nsk_render_height
    pha

    nsk_todo "To stack"

    ; Init offset
    lda _nsk_render_objy
    sta _nsk_render_offs + 0
    lda #$00
    sta _nsk_render_offs + 1

    ; y * ::NSK::SCREEN::TILEMAP::WIDTH = y << 5
    asl _nsk_render_offs + 0   ; *2
    rol _nsk_render_offs + 1

    asl _nsk_render_offs + 0   ; *4
    rol _nsk_render_offs + 1

    asl _nsk_render_offs + 0   ; *8
    rol _nsk_render_offs + 1

    asl _nsk_render_offs + 0   ; *16
    rol _nsk_render_offs + 1

    asl _nsk_render_offs + 0   ; *32
    rol _nsk_render_offs + 1

    ; y * ::NSK::SCREEN::TILEMAP::WIDTH + x

    lda _nsk_render_offs + 0
    clc
    adc _nsk_render_objx
    sta _nsk_render_offs + 0

    lda _nsk_render_offs + 1
    adc #$00
    sta _nsk_render_offs + 1

    ; _nsk_render_nametable + y * ::NSK::SCREEN::TILEMAP::WIDTH + x
    lda _nsk_render_nametable
    clc
    adc _nsk_render_offs + 0
    sta _nsk_render_offs + 0
    lda _nsk_render_nametable + 1
    adc _nsk_render_offs + 1
    sta _nsk_render_offs + 1

    height:
        nsk_todo "Extract to common PPU and assert for ::NSK::SCREEN::TILEMAP::WIDTH <> 32"

        nsk_ppu_addrset _nsk_render_offs

        ldx _nsk_render_width
        width:

            lda (_nsk_render_object), y
            iny
            sta ::NSK::CPU::PPU::PPUDATA

            dex
            bne width

        inc _nsk_render_objy

        ; Set the offset to the new Y position
        clc
        lda _nsk_render_offs
        adc #$20
        sta _nsk_render_offs
        lda _nsk_render_offs + 1
        adc #0
        sta _nsk_render_offs + 1

        nsk_ppu_addrset _nsk_render_offs

        dec _nsk_render_height
        bne height

    ; Restore clobbered values
    pla
    sta _nsk_render_height
    pla
    sta _nsk_render_objy

    rts
.endproc

; @brief Renders the object palettes
; @param[in] _nsk_render_objx      Object X position
; @param[in] _nsk_render_objy      Object Y position
; @param[in] _nsk_render_object    Object data address
; @param[in] _nsk_render_width     Object width
; @param[in] _nsk_render_height    Object height
; @warning Clobbers A, X, Y
.proc _render_object_palettes

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
    and #$01
    sta _nsk_render_quadrant_posy

    lda _nsk_render_objx
    lsr
    and #$01
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
        and #$01
        sta _nsk_render_quadrant_posy

        ; Save offset position, as the `width` loop will roll it ahead
        lda _nsk_render_offs
        pha

        ; Save object width
        lda _nsk_render_width
        pha

        width:
            ; Overflow the quadrant X
            lda _nsk_render_quadrant_posx
            and #$01
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
            lda #%11
            ldx _nsk_render_quadrant_shift
            beq :++
            :
                asl
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
                asl
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
            beq @same_x
                inc _nsk_render_offs
            @same_x:

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

        ; Increase the offset by the attribute table width:
        lda _nsk_render_quadrant_posy
        beq @same_y
            lda _nsk_render_offs
            clc
            adc #::NSK::SCREEN::ATTR::BYTE::WIDTH
            sta _nsk_render_offs
        @same_y:

        ; Nex Y quadrant
        inc _nsk_render_quadrant_posy

        ; Height check
        dec _nsk_render_height
        bne height

    rts
.endproc

; @brief Renders the object
; @param[in] _nsk_render_objx      Object X position
; @param[in] _nsk_render_objy      Object Y position
; @param[in] _nsk_render_object    Object data address
; @param[in] _nsk_render_nametable Base target nametable address
.proc _render_object
    push a, x, y
    jsr _render_object_tiles
    jsr _render_object_palettes
    pull a, x, y
    rts
.endproc

; @brief Renders the map
; @param[in] _nsk_render_mapdata   Address of the map to render
; @param[in] _nsk_render_nametable Target nametable address
.proc _render_map
    push a, x, y

    ldy #$00
    lda (_nsk_render_mapdata), y
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

    pull a, x, y
    rts
.endproc

; @brief Renders selected map on the selected nametable
; @note Requires the PPUCTRL flag VRAM_INC to be set to VRAM_INC_1
.export _nsk_render_map
.proc _nsk_render_map
    jsr _render_attrbuf_clear
    jsr _render_nametable_clear
    jsr _render_attributes_clear

    jsr _render_map

    jsr _render_attributes_write
    rts
.endproc

.endif