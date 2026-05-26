; @file nsk_render_attributes.asm
; @brief Source module for map rendering attribute routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RENDER_ATTRIBUTES_ASM__
::__NSK_RENDER_ATTRIBUTES_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_render_attributes.inc"
.include "nsk_render_vars.inc"

.segment "CODE"

; @brief Writes the _nsk_render_attrbuf buffer to the PPU
.export _render_attributes_write
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

.endif
