; @file nsk_ppu_flush.inc
; @brief Source module for the PPU flush commands
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_FLUSH_ASM__
::__NSK_PPU_FLUSH_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../ppu/nsk_ppu_flush.inc"
.include "../ppu/nsk_ppu_vars.inc"

; @brief Waits for VBlank and applies the first shadow PPU control state
.export nsk_ppu_flush_control
.proc nsk_ppu_flush_control
    bit NSK::CPU::PPU::PPUSTATUS

    wait:
        bit NSK::CPU::PPU::PPUSTATUS
        bpl wait

    lda ppu_temp_control
    sta NSK::CPU::PPU::PPUCTRL

    bit NSK::CPU::PPU::PPUSTATUS
    lda ppu_temp_scroll_x
    sta NSK::CPU::PPU::PPUSCROLL
    lda ppu_temp_scroll_y
    sta NSK::CPU::PPU::PPUSCROLL

    rts
.endproc

.endif
