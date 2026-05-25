; @file nsk_sprites_tables.inc
; @brief Include module for the sprite objects tables definitions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_TABLES__
::__NSK_SPRITES_TABLES__ = 1

.ifndef ::__NSK_SPRITES_TABLES_ASM__
    ; @brief List of the draw routines
    .import nsk_sprites_table_draw

    ; @brief List of the Out-of-bounds routines
    .import nsk_sprites_table_oob

    ; @brief List of the cleanup routines
    .import nsk_sprites_table_clean
.endif

.endif
