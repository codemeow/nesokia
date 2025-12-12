; @file nsk_init_ppu_enable.asm
; @brief Source module for the PPU enable init stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_INIT_PPU_ENABLE_ASM__
::__NSK_INIT_PPU_ENABLE_ASM__ = 1

.include "../../init/list/nsk_init_ppu_enable.inc"
.include "../../ppu/nsk_ppu_render.inc"
.include "../../ppu/nsk_ppu_nmi.inc"

.segment "CODE"

; @brief Init stage for PPU enabling
.export nsk_init_ppu_enable
.proc nsk_init_ppu_enable
    jsr nsk_ppu_render_enable
    jsr nsk_ppu_nmi_enable

    rts
.endproc

.endif