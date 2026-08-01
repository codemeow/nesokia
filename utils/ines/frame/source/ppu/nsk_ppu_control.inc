; @file nsk_ppu_control.inc
; @brief PPU NMI control interface
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_CONTROL__
::__NSK_PPU_CONTROL__ = 1

.include "../nsk_frame_configs.inc"

.if .defined(::NSK_FEATURE_PPU)          \
    .and ::NSK_FEATURE_PPU = 1           \
    .and .defined(::NSK_PPU_NMICONTROL)  \
    .and ::NSK_PPU_NMICONTROL = 1

    .ifndef ::__NSK_PPU_CONTROL_ASM__
        .import nsk_ppu_nmienable
        .import nsk_ppu_nmidisable
    .endif
.endif

.endif
