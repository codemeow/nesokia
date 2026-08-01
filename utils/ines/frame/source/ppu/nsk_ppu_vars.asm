; @file nsk_ppu_vars.asm
; @brief PPU shadow state storage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_VARS_ASM__
::__NSK_PPU_VARS_ASM__ = 1

.include "../nsk_frame_configs.inc"
.include "nsk_ppu_vars.inc"

.if .defined(::NSK_FEATURE_PPU) \
    .and ::NSK_FEATURE_PPU = 1

.segment NSK_SEGMENT_BSS

; @brief Shadow value committed to PPUCTRL by the NMI handler.
.export nsk_ppu_temp_ctrl
nsk_ppu_temp_ctrl:
    .res 1

; @brief Shadow value committed to PPUMASK by the NMI handler.
.export nsk_ppu_temp_mask
nsk_ppu_temp_mask:
    .res 1

; @brief Shadow X scroll committed by the NMI handler.
.export nsk_ppu_temp_scroll_x
nsk_ppu_temp_scroll_x:
    .res 1

; @brief Shadow Y scroll committed by the NMI handler.
.export nsk_ppu_temp_scroll_y
nsk_ppu_temp_scroll_y:
    .res 1

.endif

.endif
