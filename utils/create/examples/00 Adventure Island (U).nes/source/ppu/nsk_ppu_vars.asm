; @file nsk_ppu_vars.inc
; @brief Include module for the PPU related variables
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_VARS_ASM__
::__NSK_PPU_VARS_ASM__ = 1

.include "../ppu/nsk_ppu_vars.inc"

; @brief Temporary value for the NMI's PPUMASK
.export ppu_temp_mask
ppu_temp_mask:
    .res 1

; @brief Temporary value for the NMI's PPUCTRL
.export ppu_temp_control
ppu_temp_control:
    .res 1

; @brief Temporary value for the NMI's PPUSCROLL-X
.export ppu_temp_scroll_x
ppu_temp_scroll_x:
    .res 1

; @brief Temporary value for the NMI's PPUSCROLL-Y
.export ppu_temp_scroll_y
ppu_temp_scroll_y:
    .res 1

.endif