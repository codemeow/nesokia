; @file nsk_stage_sprites.asm
; @brief Source module for the sprites drawing
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_SPRITES_ASM__
::__NSK_STAGE_SPRITES_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "../../ppu/nsk_ppu_vars.inc"
.include "../../stage/handlers/nsk_stage_sprites.inc"
.include "../../stage/handlers/sprites/nsk_sprites_character.inc"
.include "../../stage/handlers/sprites/nsk_sprites_common.inc"
.include "../../stage/handlers/sprites/nsk_sprites_debris.inc"
.include "../../stage/handlers/sprites/nsk_sprites_hud_counter.inc"
.include "../../stage/handlers/sprites/nsk_sprites_hud_fixed.inc"

.segment "RODATA"

; @brief Table of steps required for sprites
.scope STEPS
    TABLE:
        .addr nsk_sprites_hud_fixed
        .addr nsk_sprites_hud_counter
        .addr nsk_sprites_character
        .addr nsk_sprites_debris
    _TABLE_END:

    COUNT = (STEPS::_TABLE_END - STEPS::TABLE) / 2
.endscope

; @brief Table of steps init functions
.scope INITS
    TABLE:
        .addr nsk_sprites_hud_fixed_init
        .addr nsk_sprites_hud_counter_init
        .addr nsk_sprites_character_init
        .addr nsk_sprites_debris_init
    _TABLE_END:

    COUNT = (STEPS::_TABLE_END - STEPS::TABLE) / 2
.endscope

.segment "BSS"

; @brief Index of the current step
_sprites_step_index:
    .res 1

; @brief Number of executed steps in the current frame
_sprites_step_counter:
    .res 1

; @brief Index of the starting step
; @note Should change every frame
_sprites_step_start:
    .res 1

; @brief Address storage for the jump table
_sprites_step_jumper:
    .res 2

.segment "CODE"

; @brief Draw sprites
; @note In this demo no other levels are planned, so this
;    function is a common sprites drawer
.export nsk_stage_sprites_draw
.proc nsk_stage_sprites_draw
    push a, x, y

    lda #$00
    sta _sprites_count
    sta _sprites_step_counter

    lda _sprites_step_start
    sta _sprites_step_index

    ldx #STEPS::COUNT

    loop:
        jai \
            _sprites_step_jumper,   \
            STEPS::TABLE,           \
            _sprites_step_index

        inc _sprites_step_index
        lda _sprites_step_index
        cmp #STEPS::COUNT
        bne :+
            lda #$00
            sta _sprites_step_index
        :

        dex
        bne loop


    ; Hide the rest of the sprites (if any)
    nsk_sprites_hide

    ; Rotate starting step for the next frame
    inc _sprites_step_start
    lda _sprites_step_start
    cmp #STEPS::COUNT
    bne :+
        lda #$00
        sta _sprites_step_start
    :

    pull a, x, y

    rts
.endproc

; @brief Init modules
.export nsk_stage_sprites_draw_init
.proc nsk_stage_sprites_draw_init
    push a, x

    lda #$00
    sta _sprites_step_start
    sta _sprites_step_index

    ldx #INITS::COUNT

    loop:
        jai \
            _sprites_step_jumper,   \
            INITS::TABLE,           \
            _sprites_step_index

        inc _sprites_step_index
        dex
        bne loop

    pull a, x
    rts
.endproc

.endif