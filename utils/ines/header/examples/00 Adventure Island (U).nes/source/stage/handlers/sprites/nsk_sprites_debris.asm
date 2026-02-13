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
            ;.byte $13, $14, $15, $16

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

nsk_todo "Move _pos_calc to common along with character code"

; @brief Calculates the debris visibility and screen position
;
; @param _calc_world_x Debris's world X pos
;
; @param [out] _debris_isvisible Debris is visible
; @param [out] _debris_screen_x  Debris's screen X pos (if visible)
.proc _debris_pos_calc
    push a

    ; Camera = _calc_world_x - ppu_temp_scroll_x
    sec
    lda _calc_world_x
    sbc ppu_temp_scroll_x
    pha ; Screen position if visible

    lda _calc_world_x + 1
    sbc #00 ; In this demo we don't use additional nametables,
            ; thus limiting camera to 0..255. The value here
            ; represents the "page", for instance: camera 256..512
            ; is "page" 1.

    ; visible = ([page] == 0)
    bne not_visible

    visible:
        pla
        sta _debris_screen_x
        lda #1
        sta _debris_isvisible
        jmp done

    not_visible:
        pla ; Restore anyway
        lda #0
        sta _debris_isvisible

    done:

    pull a

    rts
.endproc

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

; @brief Updates debris coordinates or respawn them
;
; param[in] X  Debris index
; param[in] Y  Layer index
.proc _debris_update
    ; Wind updates
    sec
    lda _debris_world_x_frac, x
    sbc DEBRIS::LAYERS::WIND, y
    sta _debris_world_x_frac, x

    lda _debris_world_x_lo, x
    sbc #0
    sta _debris_world_x_lo, x

    lda _debris_world_x_hi, x
    sbc #0
    sta _debris_world_x_hi, x

    ; Respawn check
    cmp #$ff
    bne :+
        jsr _debris_respawn
        jmp done
    :

    ; Gravity updates
    clc
    lda _debris_world_y_frac, x
    adc DEBRIS::LAYERS::GRAVITY, y
    sta _debris_world_y_frac, x

    lda _debris_world_y, x
    adc #0
    sta _debris_world_y, x

    ; Respawn check
    cmp #$ff
    bne :+
        jsr _debris_respawn
        jmp done
    :

    done:

    rts
.endproc

; @brief Draws or skips the debris element
;
; param[in] X                       Debris index
; param[in] Y                       Layer index
.proc _debris_draw
    lda _debris_world_x_lo, x
    sta _calc_world_x
    lda _debris_world_x_hi, x
    sta _calc_world_x + 1

    jsr _debris_pos_calc
    lda _debris_isvisible
    beq not_visible

    visible:
        nsk_sprite_draw \
            { DEBRIS::LAYERS::SPRITE, y  }, \
            { DEBRIS::LAYERS::PALETTE, y }, \
            { DEBRIS::LAYERS::ATTRS, y   }, \
            { _debris_screen_x           }, \
            { _debris_world_y, x         }

    not_visible:

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
        jsr _debris_draw

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

nsk_todo "Update 1 layer per frame"
nsk_todo "draw всё грузануть в lda, убрать .asm вообще"
nsk_todo "Уже потом попробовать тут объединить update/draw"

    rts
.endproc

.endif