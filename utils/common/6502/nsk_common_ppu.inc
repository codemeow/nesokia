; @file nsk_common_ppu.inc
; @brief Include module for the common PPU routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_COMMON_PPU__
::__NSK_COMMON_PPU__ = 1

.include "nsk_common_push.inc"
.include "nsk_common_consts.inc"

; @brief Sets the provided address to the PPUADDR
; @param [in] addr Address to set
.macro nsk_ppu_addrset addr
    push a

    bit ::NSK::CPU::PPU::PPUSTATUS
    lda addr + 1
    sta ::NSK::CPU::PPU::PPUADDR
    lda addr + 0
    sta ::NSK::CPU::PPU::PPUADDR

    pull a
.endmacro

; @brief Sets the PPU's address as palette address from the provided nametable
; @note Uses the fact that the palette address is `address` +
; `NSK::PPU::NAMETABLE::SIZE::TABLE` and the address is one of the listed:
; `$2000`, `$2400`, `$2800` or `$2c00`
.macro nsk_ppu_addrset_attrtable nametable
    push a

    bit ::NSK::CPU::PPU::PPUSTATUS
    lda nametable + 1

    clc
    adc #.hibyte(::NSK::PPU::NAMETABLE::SIZE::TABLE)
    sta ::NSK::CPU::PPU::PPUADDR

    lda #.lobyte(::NSK::PPU::NAMETABLE::SIZE::TABLE)
    sta ::NSK::CPU::PPU::PPUADDR

    pull a
.endmacro

.endif