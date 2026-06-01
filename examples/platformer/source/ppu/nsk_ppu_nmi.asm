; @file nsk_ppu_nmi.asm
; @brief Source module for the PPU NMI controls
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_NMI_ASM__
::__NSK_PPU_NMI_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "../ppu/nsk_ppu_nmi.inc"
.include "../ppu/nsk_ppu_vars.inc"

.segment "CODE"

; @brief Disables NMI handler (VBLANK)
; @note Only cancels the handler call, not the NMI itself
.export nsk_ppu_nmi_disable
.proc nsk_ppu_nmi_disable
    push a

    lda #( \
        NSK::CPU::PPU::BITS::PPUCTRL::NMI_BIT \
    )

    not
    and ppu_temp_control
    sta ppu_temp_control

    pull a
    rts
.endproc

; @brief Enables NMI handler (VBLANK)
.export nsk_ppu_nmi_enable
.proc nsk_ppu_nmi_enable
    push a

    lda #( \
        NSK::CPU::PPU::BITS::PPUCTRL::NMI_ENABLE \
    )

    ora ppu_temp_control
    sta ppu_temp_control

    pull a
    rts
.endproc

.endif