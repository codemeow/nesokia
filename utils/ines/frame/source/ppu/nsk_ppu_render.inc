; @file nsk_ppu_render.inc
; @brief PPU render control interface
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_RENDER__
::__NSK_PPU_RENDER__ = 1

.include "../nsk_frame_configs.inc"

.if .defined(::NSK_FEATURE_PPU)              \
    .and ::NSK_FEATURE_PPU = 1               \
    .and .defined(::NSK_PPU_RENDERCONTROL)   \
    .and ::NSK_PPU_RENDERCONTROL = 1

    .ifndef ::__NSK_PPU_RENDER_ASM__
        .import nsk_ppu_renderenable
        .import nsk_ppu_renderdisable
    .endif
.endif

.endif
