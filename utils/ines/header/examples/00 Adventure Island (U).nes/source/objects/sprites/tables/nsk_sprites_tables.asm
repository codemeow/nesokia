; @file nsk_sprites_tables.asm
; @brief Source module for the sprite objects tables definitions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_TABLES_ASM__
::__NSK_SPRITES_TABLES_ASM__ = 1

.include "nsk_sprites_tables.inc"

.include "../entities/nsk_sprites_hud.inc"
.include "../entities/nsk_sprites_character.inc"
.include "../entities/nsk_sprites_meteorite.inc"
.include "../entities/nsk_sprites_whirl.inc"
.include "../entities/nsk_sprites_fallingstar.inc"
.include "../entities/nsk_sprites_starbit.inc"
.include "../entities/nsk_sprites_debris.inc"
.include "../nsk_sprites_list.inc"

.segment "RODATA"

; @brief List of the object-specific tick routines
.export nsk_sprites_table_tick
nsk_sprites_table_tick:
.scope TICK
    TABLE:
        .addr $0000
        .addr nsk_character_tick
        .addr nsk_meteorite_tick
        .addr nsk_whirl_tick
        .addr nsk_fallingstar_tick
        .addr nsk_starbit_tick
        .addr $0000
        .addr $0000
        .addr $0000
        .addr $0000
    END:

    SIZE = (END - TABLE) / 2

    .assert SIZE = SPRITELIST::COUNT, error, "Sprites tick table size mismatch"
.endscope

; @brief List of the draw routines
.export nsk_sprites_table_draw
nsk_sprites_table_draw:
.scope DRAW
    TABLE:
        .addr nsk_hud_draw
        .addr nsk_character_draw
        .addr nsk_meteorite_draw
        .addr nsk_whirl_draw
        .addr nsk_fallingstar_draw
        .addr nsk_starbit_draw
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
        .addr nsk_meteorite_oob
        .addr $0000
        .addr nsk_fallingstar_oob
        .addr $0000
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
        .addr nsk_meteorite_clean
        .addr nsk_whirl_clean
        .addr nsk_fallingstar_clean
        .addr nsk_starbit_clean
        .addr $0000
        .addr $0000
        .addr $0000
        .addr $0000
    END:

    SIZE = (END - TABLE) / 2

    .assert SIZE = SPRITELIST::COUNT, error, "Sprites clean table size mismatch"
.endscope

; @brief List of the ground-check routines
;
; @note The routines get X as the pool object index. They write the result to
;       nsk_pool_result: 0 means no ground, non-zero means the object is on
;       ground. The object routine may snap its own Y position.
.export nsk_sprites_table_isonground
nsk_sprites_table_isonground:
.scope ISONGROUND
    TABLE:
        .addr $0000
        .addr nsk_character_isonground
        .addr $0000
        .addr $0000
        .addr nsk_fallingstar_isonground
        .addr nsk_starbit_isonground
        .addr $0000
        .addr $0000
        .addr $0000
        .addr $0000
    END:

    SIZE = (END - TABLE) / 2

    .assert SIZE = SPRITELIST::COUNT, error, "Sprites ground-check table size mismatch"
.endscope

; @brief List of the collision box routines
;
; @note The routines get X as the pool object index. They write the result to
;       nsk_pool_box_width and nsk_pool_box_height.
.export nsk_sprites_table_getbox
nsk_sprites_table_getbox:
.scope GETBOX
    TABLE:
        .addr $0000
        .addr nsk_character_getbox
        .addr $0000
        .addr nsk_whirl_getbox
        .addr $0000
        .addr $0000
        .addr $0000
        .addr $0000
        .addr $0000
        .addr $0000
    END:

    SIZE = (END - TABLE) / 2

    .assert SIZE = SPRITELIST::COUNT, error, "Sprites getbox table size mismatch"
.endscope

; @brief List of the collision handling routines
;
; @note The routines get X as the own pool object index and
;       nsk_pool_collision_other as the other pool object index.
.export nsk_sprites_table_collision
nsk_sprites_table_collision:
.scope COLLISION
    TABLE:
        .addr $0000
        .addr nsk_character_collision
        .addr $0000
        .addr nsk_whirl_collision
        .addr $0000
        .addr $0000
        .addr $0000
        .addr $0000
        .addr $0000
        .addr $0000
    END:

    SIZE = (END - TABLE) / 2

    .assert SIZE = SPRITELIST::COUNT, error, "Sprites collision table size mismatch"
.endscope

.endif
