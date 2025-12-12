; @file nsk_init_ppu_disable.asm
; @brief Source module for the PPU disable init stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_INIT_PPU_DISABLE_ASM__
::__NSK_INIT_PPU_DISABLE_ASM__ = 1

.include "../../init/list/nsk_init_ppu_disable.inc"
.include "../../ppu/nsk_ppu_render.inc"
.include "../../ppu/nsk_ppu_nmi.inc"

.segment "CODE"

; @brief Init stage for PPU disabling
.export nsk_init_ppu_disable
.proc nsk_init_ppu_disable
    jsr nsk_ppu_render_disable
    jsr nsk_ppu_nmi_disable

    rts
.endproc

.endif