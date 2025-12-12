; @file nsk_ppu_flush.inc
; @brief Include module for the PPU flush commands
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_FLUSH__
::__NSK_PPU_FLUSH__ = 1

.ifndef ::__NSK_PPU_FLUSH_ASM__
; @brief Flushes the current ppu_temp_control variable directly to PPU
.import nsk_ppu_flush_control
.endif

.ifndef ::__NSK_PPU_FLUSH_ASM__
; @brief Flushes the current ppu_temp_mask variable directly to PPU
.import nsk_ppu_flush_mask
.endif

.endif