; @file nsk_ppu_palette.inc
; @brief Include module for the PPU palette control
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_PPU_PALETTE__
::__NSK_PPU_PALETTE__ = 1

.include "nsk_common_meta.inc"

.ifndef ::__NSK_PPU_PALETTE_ASM__
; @brief Temporary address to store palette data address
.importzp _nsk_ppu_palette_data
.endif

.ifndef ::__NSK_PPU_PALETTE_ASM__
; @brief Temporary address to store ppu address
.import _nsk_ppu_palette_addr
.endif

.ifndef ::__NSK_PPU_PALETTE_ASM__
; @brief Sets the palette (both tiles and sprites)
.import _nsk_ppu_palette_set
.endif

; @brief Sets the palette for tiles
; @param [in] address   PPU address to use
; @param [in] palette   Palette address for the tiles
.macro nsk_palette_set address, palette
    nsk_p16_set _nsk_ppu_palette_data, palette, 16
    nsk_p16_set _nsk_ppu_palette_addr, address, 16

    jsr _nsk_ppu_palette_set
.endmacro

; @brief Sets the palette for tiles
; @param [in] palette   Palette address for the tiles
.macro nsk_palette_tilesset palette
    nsk_palette_set #NSK::PPU::PALETTE::TILES, palette
.endmacro

; @brief Sets the palette for sprites
; @param [in] palette Palette address for the sprites
.macro nsk_palette_spritesset palette
    nsk_palette_set #NSK::PPU::PALETTE::SPRITES, palette
.endmacro

; @brief Sets the palettes
; @param [in] palette_tiles   Palette address for the tiles
; @param [in] palette_sprites Palette address for the sprites
.macro nsk_ppu_palettes_set palette_tiles, palette_sprites
    nsk_palette_tilesset   #palette_tiles
    nsk_palette_spritesset #palette_sprites
.endmacro

.endif