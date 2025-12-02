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

.segment "BSS"

; @brief Address for the selected nametable
.export _nsk_render_nametable
_nsk_render_nametable:
    .res 2

; @brief Address for the selected map data
.export _nsk_render_mapdata
_nsk_render_mapdata:
    .res 2

; @brief Attributes buffer
_nsk_render_attrbuf:
    .res NSK_ATTRTABLE_SIZE

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

    ldy #NSK::SCREEN::TILEMAP::HEIGHT
    rows:
        ldx #NSK::SCREEN::TILEMAP::WIDTH

        cols:
            sta NSK::CPU::PPU::PPUDATA
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

    lda #$00
    ldx #NSK::PPU::NAMETABLE::SIZE::ATTRS
    loop:
        sta NSK::CPU::PPU::PPUDATA
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
        sta NSK::CPU::PPU::PPUDATA
        inx
        cpx #NSK::PPU::NAMETABLE::SIZE::ATTRS
        bne loop

    pull a, x
    rts
.endproc

; @brief Renders selected map on the selected nametable
; @note Requires the PPUCTRL flag VRAM_INC to be set to VRAM_INC_1
.export _nsk_render_map
.proc _nsk_render_map
    ;jsr _render_attrbuf_clear
    ;jsr _render_nametable_clear
    ;jsr _render_attributes_clear

    nsk_todo "_nsk_render_map"

    ;jsr _render_attributes_write
    rts
.endproc

.endif