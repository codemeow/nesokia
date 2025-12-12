; @file nsk_init_tiles_draw.asm
; @brief Source module for the tiles drawing init stage
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_INIT_TILES_DRAW_ASM__
::__NSK_INIT_TILES_DRAW_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../init/list/nsk_init_tiles_draw.inc"
.include "../../ppu/nsk_ppu_palette.inc"
.include "../../stage/assets/nsk_asset_palette.inc"
.include "../../stage/maps/nsk_map_data.inc"
.include "../../stage/render/nsk_render_map.inc"

.segment "CODE"

; @brief Init stage for tiles drawing
; @warning Uses raw OAM access, requires rendering/NMI to be disabled
.export nsk_init_tiles_draw
.proc nsk_init_tiles_draw
    nsk_render_map #NSK::PPU::NAMETABLE::TABLE0, nsk_map_left
    nsk_render_map #NSK::PPU::NAMETABLE::TABLE1, nsk_map_right
    nsk_ppu_palettes_set nsk_palette_back, nsk_palette_sprites

    rts
.endproc

.endif