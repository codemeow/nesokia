; @file nsk_ppu_render.asm
; @brief PPU render shadow control
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_RENDER_ASM__
::__NSK_PPU_RENDER_ASM__ = 1

.include "nsk_common_meta.inc"
.include "../nsk_frame_configs.inc"
.include "nsk_ppu_render.inc"
.include "nsk_ppu_vars.inc"

.if .defined(::NSK_FEATURE_PPU)              \
    .and ::NSK_FEATURE_PPU = 1               \
    .and .defined(::NSK_PPU_RENDERCONTROL)   \
    .and ::NSK_PPU_RENDERCONTROL = 1

.segment NSK_SEGMENT_PPUCODE

; @brief Disables background and sprite rendering in the PPU mask shadow value.
; @details The value is committed by the configured NMI update path.
.export nsk_ppu_renderdisable
.proc nsk_ppu_renderdisable
    push a

    lda #( \
        NSK::CPU::PPU::BITS::PPUMASK::RENDER_BACK_BIT    | \
        NSK::CPU::PPU::BITS::PPUMASK::RENDER_SPRITES_BIT \
    )
    not
    and nsk_ppu_temp_mask
    sta nsk_ppu_temp_mask

    pull a
    rts
.endproc

; @brief Enables background and sprite rendering in the PPU mask shadow value.
; @details The value is committed by the configured NMI update path.
.export nsk_ppu_renderenable
.proc nsk_ppu_renderenable
    push a

    lda #( \
        NSK::CPU::PPU::BITS::PPUMASK::RENDER_BACK_ON    | \
        NSK::CPU::PPU::BITS::PPUMASK::RENDER_SPRITES_ON \
    )
    ora nsk_ppu_temp_mask
    sta nsk_ppu_temp_mask

    pull a
    rts
.endproc

.endif

.endif
