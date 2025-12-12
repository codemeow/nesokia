; @file nsk_asset_palette.inc
; @brief Include module for the palettes list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_ASSET_PALETTE__
::__NSK_ASSET_PALETTE__ = 1

.ifndef ::__NSK_ASSET_PALETTE_ASM__
; @brief Background palette
.import nsk_palette_back
.endif

.ifndef ::__NSK_ASSET_PALETTE_ASM__
; @brief Sprites palette
.import nsk_palette_sprites
.endif

.endif