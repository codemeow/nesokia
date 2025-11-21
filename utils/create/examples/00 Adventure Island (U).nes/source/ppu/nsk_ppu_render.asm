; @file nsk_ppu_render.asm
; @brief Source module for the PPU render controls
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_RENDER_ASM__
::__NSK_PPU_RENDER_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "../ppu/nsk_ppu_render.inc"
.include "../ppu/nsk_ppu_vars.inc"

.segment "CODE"

; @brief Disables PPU rendering (tiles and sprites) withing next VBLANK
.export nsk_ppu_render_disable
.proc nsk_ppu_render_disable
    push a

    lda #( \
        NSK::PPU::BITS::PPUMASK::RENDER_BACK_BIT    | \
        NSK::PPU::BITS::PPUMASK::RENDER_SPRITES_BIT   \
    )

    not
    and ppu_temp_mask
    sta ppu_temp_mask

    pull a
    rts
.endproc

; @brief Enables PPU rendering (tiles and sprites) withing next VBLANK
.export nsk_ppu_render_enable
.proc nsk_ppu_render_enable
    push a

    lda #( \
        NSK::PPU::BITS::PPUMASK::RENDER_BACK_ON    | \
        NSK::PPU::BITS::PPUMASK::RENDER_SPRITES_ON   \
    )

    ora ppu_temp_mask
    sta ppu_temp_mask

    pull a
    rts
.endproc

.endif