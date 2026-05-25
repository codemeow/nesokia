; @file nsk_sprites_tables.asm
; @brief Source module for the sprite objects tables definitions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_TABLES_ASM__
::__NSK_SPRITES_TABLES_ASM__ = 1

.include "nsk_sprites_tables.inc"

.include "../entities/nsk_sprites_hud.inc"
.include "../entities/nsk_sprites_character.inc"
.include "../entities/nsk_sprites_debris.inc"
.include "../nsk_sprites_list.inc"

.segment "RODATA"

; @brief List of the draw routines
.export nsk_sprites_table_draw
nsk_sprites_table_draw:
.scope DRAW
    TABLE:
        .addr nsk_hud_draw
        .addr nsk_character_draw
        .addr nsk_debris_draw
        .addr nsk_debris_draw
        .addr nsk_debris_draw
        .addr nsk_debris_draw
    END:

    SIZE = (END - TABLE) / 2

    .assert SIZE = SPRITELIST::COUNT, error, "Sprites draw table size mismatch"
.endscope

; @brief List of the Out-of-bounds routines
.export nsk_sprites_table_oob
nsk_sprites_table_oob:
.scope OOB
    TABLE:
        .addr $0000
        .addr nsk_character_oob
        .addr nsk_debris_oob
        .addr nsk_debris_oob
        .addr nsk_debris_oob
        .addr nsk_debris_oob
    END:

    SIZE = (END - TABLE) / 2

    .assert SIZE = SPRITELIST::COUNT, error, "Sprites OOB table size mismatch"
.endscope

; @brief List of the cleanup routines
.export nsk_sprites_table_clean
nsk_sprites_table_clean:
.scope CLEAN
    TABLE:
        .addr $0000
        .addr nsk_character_clean
        .addr $0000
        .addr $0000
        .addr $0000
        .addr $0000
    END:

    SIZE = (END - TABLE) / 2

    .assert SIZE = SPRITELIST::COUNT, error, "Sprites clean table size mismatch"
.endscope

.endif
