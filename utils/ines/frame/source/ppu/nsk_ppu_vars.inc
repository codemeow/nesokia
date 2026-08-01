; @file nsk_ppu_vars.inc
; @brief PPU shadow state interface
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_VARS__
::__NSK_PPU_VARS__ = 1

.include "../nsk_frame_configs.inc"

.if .defined(::NSK_FEATURE_PPU) \
    .and ::NSK_FEATURE_PPU = 1

    .ifndef ::__NSK_PPU_VARS_ASM__
        .import nsk_ppu_temp_ctrl
        .import nsk_ppu_temp_mask
        .import nsk_ppu_temp_scroll_x
        .import nsk_ppu_temp_scroll_y
    .endif
.endif

.endif
