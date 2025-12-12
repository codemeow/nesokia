; @file nsk_ppu_nmi.inc
; @brief Include module for the PPU NMI controls
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_NMI__
::__NSK_PPU_NMI__ = 1

.ifndef ::__NSK_PPU_NMI_ASM__
; @brief Disables NMI handler (VBLANK)
; @note Only cancels the handler call, not the NMI itself
.import nsk_ppu_nmi_disable
.endif

.ifndef ::__NSK_PPU_NMI_ASM__
; @brief Enables NMI handler (VBLANK)
.import nsk_ppu_nmi_enable
.endif

.endif