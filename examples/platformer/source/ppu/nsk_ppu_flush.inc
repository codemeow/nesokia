; @file nsk_ppu_flush.inc
; @brief Include module for the PPU flush commands
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_FLUSH__
::__NSK_PPU_FLUSH__ = 1

.ifndef ::__NSK_PPU_FLUSH_ASM__
; @brief Waits for VBlank and applies the first shadow PPU control state
.import nsk_ppu_flush_control
.endif

.endif
