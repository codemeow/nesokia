; @file nsk_ppu_vars.inc
; @brief Include module for the PPU related variables
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_VARS__
::__NSK_PPU_VARS__ = 1

.ifndef ::__NSK_PPU_VARS_ASM__
; @brief Temporary value for the NMI's PPUMASK
.import ppu_temp_mask
.endif

.ifndef ::__NSK_PPU_VARS_ASM__
; @brief Temporary value for the NMI's PPUCTRL
.import ppu_temp_control
.endif

.ifndef ::__NSK_PPU_VARS_ASM__
; @brief Temporary value for the NMI's PPUSCROLL-X
.import ppu_temp_scroll_x
.endif

.ifndef ::__NSK_PPU_VARS_ASM__
; @brief Temporary value for the NMI's PPUSCROLL-Y
.import ppu_temp_scroll_y
.endif

.endif