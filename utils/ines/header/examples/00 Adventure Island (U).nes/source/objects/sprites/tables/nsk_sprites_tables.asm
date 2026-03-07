; @file nsk_sprites_tables.asm
; @brief Source module for the sprite objects tables definitions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_TABLES_ASM__
::__NSK_SPRITES_TABLES_ASM__ = 1

.include "nsk_sprites_tables.inc"

.include "../entities/nsk_sprites_hud.inc"
.include "../nsk_sprites_list.inc"
.include "nsk_tables_helpers.inc"

.segment "RODATA"

; @brief List of the draw routines
.export nsk_sprites_table_draw
nsk_sprites_table_draw:
    _NSK_TABLE_START
    _NSK_TABLE_LINE  SPRITELIST::HUD,   nsk_hud_draw
    _NSK_TABLE_COUNT SPRITELIST::COUNT

.endif