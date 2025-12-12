; @file nsk_ppu_render.inc
; @brief Include module for the PPU render controls
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_RENDER__
::__NSK_PPU_RENDER__ = 1

.ifndef ::__NSK_PPU_RENDER_ASM__
; @brief Disables PPU rendering (tiles and sprites) withing next VBLANK
.import nsk_ppu_render_disable
.endif

.ifndef ::__NSK_PPU_RENDER_ASM__
; @brief Enables PPU rendering (tiles and sprites) withing next VBLANK
.import nsk_ppu_render_enable
.endif

.endif