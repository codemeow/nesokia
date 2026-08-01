; @file nsk_ppu_control.asm
; @brief PPU NMI shadow control
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_CONTROL_ASM__
::__NSK_PPU_CONTROL_ASM__ = 1

.include "nsk_common_meta.inc"
.include "../nsk_frame_configs.inc"
.include "nsk_ppu_control.inc"
.include "nsk_ppu_vars.inc"

.if .defined(::NSK_FEATURE_PPU)          \
    .and ::NSK_FEATURE_PPU = 1           \
    .and .defined(::NSK_PPU_NMICONTROL)  \
    .and ::NSK_PPU_NMICONTROL = 1

.segment NSK_SEGMENT_PPUCODE

; @brief Disables NMI generation in the PPU control shadow value.
; @details The value is committed by the configured NMI update path.
.export nsk_ppu_nmidisable
.proc nsk_ppu_nmidisable
    push a

    lda #NSK::CPU::PPU::BITS::PPUCTRL::NMI_BIT
    not
    and nsk_ppu_temp_ctrl
    sta nsk_ppu_temp_ctrl

    pull a
    rts
.endproc

; @brief Enables NMI generation in the PPU control shadow value.
; @details The value is committed by the configured NMI update path.
.export nsk_ppu_nmienable
.proc nsk_ppu_nmienable
    push a

    lda #NSK::CPU::PPU::BITS::PPUCTRL::NMI_ENABLE
    ora nsk_ppu_temp_ctrl
    sta nsk_ppu_temp_ctrl

    pull a
    rts
.endproc

.endif

.endif
