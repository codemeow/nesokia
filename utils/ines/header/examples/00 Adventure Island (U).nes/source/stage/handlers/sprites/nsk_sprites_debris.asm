; @file nsk_sprites_debris.asm
; @brief Source module for the sprites debris
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_DEBRIS_ASM__
::__NSK_SPRITES_DEBRIS_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "../../../stage/handlers/sprites/nsk_sprites_debris.inc"
.include "../../../stage/handlers/sprites/nsk_sprites_common.inc"
.include "../../../ppu/nsk_ppu_vars.inc"
.include "../../../utils/nsk_util_rand8.inc"

.segment "RODATA"

; Number of debris per layer
; Exported out of the table for .res declarations
_LAYER_0_COUNT = 10
_LAYER_1_COUNT = 8
_LAYER_2_COUNT = 6
_LAYER_3_COUNT = 4

; Total number of debris objects
_DEBRIS_TOTAL = \
    _LAYER_0_COUNT + _LAYER_1_COUNT + \
    _LAYER_2_COUNT + _LAYER_3_COUNT

; Debris settings
.scope DEBRIS

    ; Layers settings
    .scope LAYERS
        ; 0 - The farthest
        SPRITE:
            .byte $bf, $bf, $be, $be

        ; Number of sprites per layer
        SIZE:
            .byte _LAYER_0_COUNT, _LAYER_1_COUNT
            .byte _LAYER_2_COUNT, _LAYER_3_COUNT

        ; Sprites attributes
        ATTRS:
            .byte NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::BACKGROUND
            .byte NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::BACKGROUND
            .byte NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::BACKGROUND
            .byte NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND

        ; Sprites palettes
        PALETTE:
            .byte %00, %00, %00, %00

        ; Constant right-to-left force
        WIND:
            .byte 112, 130, 151, 223

        ; Constant top-to-down force
        GRAVITY:
            .byte 10, 17, 31, 90

        ; Number of layers count
        COUNT = 4
    .endscope

    ; Total number of debris
    COUNT = _DEBRIS_TOTAL
.endscope

.segment "BSS"

; Debris instances

; @brief World X position (3 bytes)
_debris_world_x_frac:
    .res _DEBRIS_TOTAL
_debris_world_x_lo:
    .res _DEBRIS_TOTAL
_debris_world_x_hi:
    .res _DEBRIS_TOTAL

; @brief World (and screen) Y position (2 bytes)
_debris_world_y_frac:
    .res _DEBRIS_TOTAL
_debris_world_y:
    .res _DEBRIS_TOTAL

; @brief If set the current debris is on the screen
_debris_isvisible:
    .res 1

; @brief Current debris X screen position
_debris_screen_x:
    .res 1

; @brief Temp var for pos calc input
_calc_world_x:
    .res 2

; @brief Address storage for the jump table
_debris_layers_jumper:
    .res 2

; @brief Index of the current debris
_debris_index:
    .res 1
; @brief Index of the current layer
_debris_layer_index:
    .res 1
; @brief Index of the debris in layer
_debris_inlayer_index:
    .res 1

.segment "CODE"

; @brief Respawns current debris
;
; param[in] X  Debris index
; param[in] Y  Layer index
.proc _debris_respawn
    ; Hi
    lda #1
    sta _debris_world_x_hi, x

    ; Lo
    lda #$ff
    sta _debris_world_x_lo, x

    ; Frac
    lda #0
    sta _debris_world_x_frac, x

    ; Y
    jsr nsk_util_rand8
    sta _debris_world_y, x

    lda #0
    sta _debris_world_y_frac, x

    rts
.endproc

; @brief Process single debris
;
; @param[in] X                      Index of the debris (total)
; @param[in] Y                      Layer index
; @param[in] _debris_inlayer_index  Index of the debris in the current layer
.proc _debris_update
    ; United logic within one proc as separate procs require too much of API
    ; connection between them that consumes too much (in my view) time

    ; # Constant wind
    ; @note Could be splitted between frames later
    sec
    lda _debris_world_x_frac, x
    sbc DEBRIS::LAYERS::WIND, y
    sta _debris_world_x_frac, x

    ; In case of splitting the low part could also be coded in the DEBRIS::
    lda _debris_world_x_lo, x
    sbc #0
    sta _debris_world_x_lo, x

    lda _debris_world_x_hi, x
    sbc #0
    sta _debris_world_x_hi, x

    cmp #$ff
    bne :+
        jsr _debris_respawn
        jmp done
    :

    ; # Constant gravity
    ; @note Could also be splitted between frames later
    clc
    lda _debris_world_y_frac, x
    adc DEBRIS::LAYERS::GRAVITY, y
    sta _debris_world_y_frac, x

    lda _debris_world_y, x
    adc #0
    sta _debris_world_y, x

    ; Could also be "clamped" here by the values in DEBRIS::LAYERS::MAXY
    cmp #$ff
    bne :+
        jsr _debris_respawn
        jmp done
    :

    ; # Visibility
    ; @note The calculation itself could also be splitted between frames
    lda _debris_world_x_lo, x
    sec
    sbc ppu_temp_scroll_x
    sta _debris_screen_x

    ; Carry flag meanings after SBC:
    ;   C = 1 : no borrow : world_x >= scroll_x
    ;   C = 0 : borrow    : world_x <  scroll_x
    lda _debris_world_x_hi, x

    beq hi0

    ; hi != 0:
    ;   world_x is either far right of screen,
    ;   or slightly left but hi-byte was already non-zero
    hi1:
        ; No borrow : world_x > visible area (right)
        bcs not_visible
        ; Borrow: within visible area
        jmp visible

    ; hi == 0:
    ;   world_x is within or left of screen
    hi0:
        ; Borrow : world_x < scroll_x (left of screen)
        bcc not_visible
        ; No borrow: within visible area

    ; # Drawing the sprites
    ; This is the only block that cannot be splitted
    visible:
        nsk_sprite_draw \
            { DEBRIS::LAYERS::SPRITE, y  }, \
            { DEBRIS::LAYERS::ATTRS, y   }, \
            { DEBRIS::LAYERS::PALETTE, y }, \
            { _debris_screen_x           }, \
            { _debris_world_y, x         }

    not_visible:
        ; Doing nothing

    done:

    rts
.endproc

; @brief Draws the debris
.export nsk_sprites_debris
.proc nsk_sprites_debris
    push a, x, y

    lda #0
    sta _debris_inlayer_index

    ; Debris index (sum)
    ldx #0
    ; Layer index
    ldy #0

    loop:
        jsr _debris_update

        inc _debris_inlayer_index
        lda _debris_inlayer_index
        cmp DEBRIS::LAYERS::SIZE, y
        bne :+
            iny
            lda #0
            sta _debris_inlayer_index
        :

        inx
        cpx #DEBRIS::COUNT
        bne loop

    pull a, x, y

    rts
.endproc

; @brief Init module
.export nsk_sprites_debris_init
.proc nsk_sprites_debris_init
    push a, x

    ; Debris index (sum)
    ldx #0

    loop:
        ; Hi
        lda #0
        sta _debris_world_x_hi, x

        jsr nsk_util_rand8
        cmp #$7f
        bcs :+
            inc _debris_world_x_hi, x
        :

        ; Lo
        jsr nsk_util_rand8
        sta _debris_world_x_lo, x

        ; Frac
        jsr nsk_util_rand8
        sta _debris_world_x_frac, x

        ; Y
        jsr nsk_util_rand8
        sta _debris_world_y, x

        lda #0
        sta _debris_world_y_frac, x

        inx
        cpx #DEBRIS::COUNT
        bne loop

    pull a, x

    rts
.endproc

.endif