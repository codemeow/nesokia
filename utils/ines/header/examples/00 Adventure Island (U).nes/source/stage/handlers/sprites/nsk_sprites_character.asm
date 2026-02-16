; @file nsk_sprites_character.asm
; @brief Source module for the character drawing routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_CHARACTER_ASM__
::__NSK_SPRITES_CHARACTER_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "../../../stage/handlers/sprites/nsk_sprites_character.inc"
.include "../../../stage/handlers/sprites/nsk_sprites_common.inc"
.include "../../../ppu/nsk_ppu_vars.inc"

.segment "RODATA"

; @brief Character visual data
.scope CHARACTER
    POSX:
        .byte 0,8,0,8,0,8
    POSY:
        .byte 0,0,8,8,16,16
    SPRITE:
        .byte $48,$49,$58,$59,$68,$69
    ATTRS = \
            NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
            NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::NO        | \
            NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO
    PALETTE = %10

    SPRITES = 6
.endscope

.segment "BSS"

; "World" coords for the character
_character_world_x:
    .res 2
; "World" (=screen) coords for the character
_character_world_y:
    .res 1

; If set the character is on the screen
_character_isvisible:
    .res 1
_character_screen_x:
    .res 1

; Temp var for Y camera pos
_temp_posy:
    .res 1

; Temp var for pos calc input
_calc_world_x:
    .res 2

.segment "CODE"

; @brief Calculates the character visibility and screen position
;
; @param _calc_world_x Character's world X pos
;
; @param [out] _character_isvisible Character is visible
; @param [out] _character_screen_x  Character's screen X pos (if visible)
.proc _character_pos_calc
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
        sta _character_screen_x
        lda #1
        sta _character_isvisible
        jmp done

    not_visible:
        pla ; Restore anyway
        lda #0
        sta _character_isvisible

    done:

    pull a

    rts
.endproc

; @brief Draws the character
.export nsk_sprites_character
.proc nsk_sprites_character
    push a, x

    ldx #0

    loop:
        ; X position
        clc
        lda _character_world_x
        adc CHARACTER::POSX, x
        sta _calc_world_x

        lda _character_world_x + 1
        adc #0
        sta _calc_world_x + 1

        ; Check if visible
        jsr _character_pos_calc
        lda _character_isvisible
        beq not_visible

        visible:
            ; Y position
            clc
            lda _character_world_y
            adc CHARACTER::POSY, x
            sta _temp_posy

            nsk_sprite_draw \
                { CHARACTER::SPRITE, x  }, \
                { #CHARACTER::ATTRS     }, \
                { #CHARACTER::PALETTE   }, \
                { _character_screen_x   }, \
                { _temp_posy            }

        not_visible:

        inx
        cpx #CHARACTER::SPRITES
        bne loop

    done:

    pull a, x
    rts
.endproc

; @brief Inits module
.export nsk_sprites_character_init
.proc nsk_sprites_character_init
    lda #232
    sta _character_world_x + 0
    lda #0
    sta _character_world_x + 1

    lda #167
    sta _character_world_y

    rts
.endproc

.endif