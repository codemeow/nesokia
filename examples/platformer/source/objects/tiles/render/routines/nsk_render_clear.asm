; @file nsk_render_clear.asm
; @brief Source module for map rendering clear routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_CLEAR_ASM__
::__NSK_RENDER_CLEAR_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_render_clear.inc"
.include "nsk_render_vars.inc"
.include "../../maps/nsk_map_vars.inc"

.segment "CODE"

; @brief Clears the nametables' attributes buffer
.export _render_attrbuf_clear
.proc _render_attrbuf_clear
    push a, x

    lda #$00
    ldx #$00

    loop:
        sta _nsk_render_attrbuf, x
        inx
        cpx #::NSK::PPU::NAMETABLE::SIZE::ATTRS
        bne loop

    pull a, x
    rts
.endproc

; @brief Clears the nametables' tiles
; Sets the whole tile background to $00 tile
.export _render_nametable_clear
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

; @brief Resets the collision data
.export _nsk_render_collision_clear
.proc _nsk_render_collision_clear
    push a, x

    .assert \
        ::MAP::SCREEN::PAGE <= 256, \
        error,                      \
        "::MAP::SCREEN::PAGE should be less or equal to 256"

    lda #0
    ldx #0

    :
        .repeat MAP::SCREEN::PAGES, i
            _shift .set i * ::MAP::SCREEN::PAGE

            sta nsk_map_grid + _shift, x
        .endrep

        inx
        .if ::MAP::SCREEN::PAGE < 256
            cpx #::MAP::SCREEN::PAGE
        .endif
        bne :-

    pull a, x

    rts
.endproc

.endif
