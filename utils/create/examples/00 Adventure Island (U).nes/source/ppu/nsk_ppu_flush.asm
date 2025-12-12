; @file nsk_ppu_flush.inc
; @brief Source module for the PPU flush commands
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_FLUSH_ASM__
::__NSK_PPU_FLUSH_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../ppu/nsk_ppu_flush.inc"
.include "../ppu/nsk_ppu_vars.inc"

; @brief Flushes the current ppu_temp_control variable directly to PPU
.export nsk_ppu_flush_control
.proc nsk_ppu_flush_control
    lda ppu_temp_control
    sta NSK::CPU::PPU::PPUCTRL
.endproc

; @brief Flushes the current ppu_temp_mask variable directly to PPU
.export nsk_ppu_flush_mask
.proc nsk_ppu_flush_mask
    lda ppu_temp_mask
    sta NSK::CPU::PPU::PPUMASK
.endproc

.endif