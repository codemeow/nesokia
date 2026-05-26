; @file nsk_render_object_tiles.asm
; @brief Source module for map object tile rendering
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_OBJECT_TILES_ASM__
::__NSK_RENDER_OBJECT_TILES_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_render_object_tiles.inc"
.include "nsk_render_vars.inc"

.segment "CODE"

.assert \
    ::NSK::SCREEN::TILEMAP::WIDTH = 32, \
    error,                              \
    "Object tile renderer expects 32-tile-wide nametables"

; @brief Renders the object tiles
; @param[in] _nsk_render_objx      Object X position
; @param[in] _nsk_render_objy      Object Y position
; @param[in] _nsk_render_object    Object data address
; @param[in] _nsk_render_nametable Base target nametable address
; @warning Clobbers A, X, Y
.export _render_object_tiles
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

    ; Init offset
    lda _nsk_render_objy
    sta _nsk_render_offs + 0
    lda #0
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
        nsk_todo "Extract nametable row increment to common PPU helper"

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
        adc #::NSK::SCREEN::TILEMAP::WIDTH
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

.endif
