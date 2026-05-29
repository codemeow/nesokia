; @file nsk_ppu_palette.asm
; @brief Source module for the PPU palette control
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_PALETTE_ASM__
::__NSK_PPU_PALETTE_ASM__ = 1

.include "nsk_common_meta.inc"

.segment "ZEROPAGE"

; @brief Temporary address to store palette data address
.exportzp _nsk_ppu_palette_data
_nsk_ppu_palette_data:
    .res 2

.segment "BSS"

; @brief Temporary address to store ppu address
.export _nsk_ppu_palette_addr
_nsk_ppu_palette_addr:
    .res 2

.segment "CODE"

; @brief Waits until VBlank starts
.proc _vblank_wait
    bit NSK::CPU::PPU::PPUSTATUS

    wait:
        bit NSK::CPU::PPU::PPUSTATUS
        bpl wait

    rts
.endproc

; @brief Moves PPUADDR away from palette RAM after forced-blank writes
.proc _ppu_addr_reset
    bit NSK::CPU::PPU::PPUSTATUS

    lda #$00
    sta NSK::CPU::PPU::PPUADDR
    sta NSK::CPU::PPU::PPUADDR

    rts
.endproc

; @brief Sets the palette (both tiles and sprites)
.export _nsk_ppu_palette_set
.proc _nsk_ppu_palette_set
    push a, y

    jsr _vblank_wait
    nsk_ppu_addrset _nsk_ppu_palette_addr

    ldy #$00

    loop:
        lda (_nsk_ppu_palette_data), y
        sta NSK::CPU::PPU::PPUDATA
        iny
        cpy #NSK::PPU::PALETTE::SIZE
        bne loop

    jsr _ppu_addr_reset

    pull a, y
    rts
.endproc

.endif
