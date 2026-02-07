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
.include "../../utils/nsk_util_bcd.inc"

.segment "RODATA"

;@brief HUD-related settings
;
; @note The actual Y position should be 1 pixel less, but in the purpose of demo
;       and readablity I keep the original values
.scope HUD
    .scope CAPTION
        posx:
            .byte 16,24,32
        posy:
            .byte 24,24,24
        sprite:
            .byte $13,$14,$15
        palette:
            .byte %00,%00,%00

        ; Same attributes for all sprites in purpose of simplicity
        ATTRS = \
            NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
            NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::NO        | \
            NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO
        COUNT = 3   ; Number of fixed HUD sprites
    .endscope

    .scope COUNTER
        posx:
            .byte 48,56,64,72
        POSY_TOP    = 16 ; Top row Y position
        POSY_BOTTOM = 24 ; Bottom row Y position

        sprite_top:
            .byte $06,$07,$08,$09,$0a, $0b,$0c,$0d,$0e,$0f
        sprite_bottom:
            .byte $16,$17,$18,$19,$1a, $1b,$1c,$1d,$1e,$1f

        ; Same attributes for all sprites in purpose of simplicity
        ATTRS = \
            NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
            NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::NO        | \
            NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO
        ; Same palette for all digits
        PALETTE = %00
    .endscope
.endscope

; @brief List of steps for the sprites drawing
_sprites_steps:
    .addr _sprites_draw_hud_fixed
    .addr _sprites_draw_hud_counter
_sprites_steps_end:

; @brief Number of steps in steps table
_SPRITES_STEPS_COUNT = (_sprites_steps_end - _sprites_steps) / 2

.segment "BSS"

; @brief How many sprites are active in this frame
_sprites_count:
    .res 1

; @brief Current number of "souls"
_hud_souls:
    .res 1

; @brief Number of digits in the "souls" counter as decimal
_souls_digits:
    .res 1

; @brief Sprite index for `_sprite_draw`
_sprite_index:
    .res 1
; @brief Sprite attribute byte for `_sprite_draw`
_sprite_attr:
    .res 1
; @brief Sprite posx for `_sprite_draw`
_sprite_posx:
    .res 1
; @brief Sprite posy for `_sprite_draw`
_sprite_posy:
    .res 1

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

; @brief Draws the sprite by stored values:
;
; @param _sprite_index Sprite index
; @param _sprite_attr  Sprite attribute byte
; @param _sprite_posx  Sprite X position
; @param _sprite_posy  Sprite Y position
; @param X             OAM shift in bytes
.proc _sprite_draw_in
    lda _sprite_posy
    sta NSK::CPU::RAM::OAMBUFFER, x
    inx

    lda _sprite_index
    sta NSK::CPU::RAM::OAMBUFFER, x
    inx

    lda _sprite_attr
    sta NSK::CPU::RAM::OAMBUFFER, x
    inx

    lda _sprite_posx
    sta NSK::CPU::RAM::OAMBUFFER, x
    inx

    rts
.endproc

; @brief Draws one single tile
; @param index      Tile index
; @param palette    Tile palette
; @param attributes Tile attributes
; @param posx       Tile X position
; @param posy       Tile Y position
;
; @param _sprites_count Number of used sprites
;
; @note Checks on _sprites_count. If it's equal to NSK::PPU::SPRITES::MAXCOUNT,
;       exits instead
.macro _sprite_draw index, palette, attributes, posx, posy
.scope
    push a, x

    lda _sprites_count
    cmp #NSK::PPU::SPRITES::MAXCOUNT
    beq done

    ; Buffer the values to allow the { aaa, x }/{ aaa, y } input
    lda index
    sta _sprite_index

    lda palette
    ora attributes
    sta _sprite_attr

    lda posx
    sta _sprite_posx

    lda posy
    sta _sprite_posy

    ; Convert number of sprites into OAM shift
    lda _sprites_count
    asl
    asl
    tax

    jsr _sprite_draw_in

    inc _sprites_count

    done:

    pull a, x
.endscope
.endmacro

; @brief Hides the unused sprite
;
; @param _sprites_count Index of the slot
.macro _sprite_hide
    push a, x

    lda _sprites_count
    asl
    asl
    tax

    lda #$ff
    sta NSK::CPU::RAM::OAMBUFFER, x

    inc _sprites_count

    pull a, x
.endmacro

; @brief Counts the number of digits in the result
.proc _souls_digits_count
    push a, x

    ldx #1

    lda nsk_bcd_outt
    beq :+
        ldx #2
    :
    lda nsk_bcd_outh
    beq :+
        ldx #3
    :
    stx _souls_digits

    pull a, x

    rts
.endproc

; @brief Calculates everything needed to draw "souls" counter
.proc _sprites_draw_hud_counter
    push a, x, y

    ; For the demo purpose we will use the scrolling position for the counter
    lda ppu_temp_scroll_x
    jsr nsk_bin8_todec

    jsr _souls_digits_count

    ; X: Number of draw digits
    ; Used as shift in the positions arrays
    ldx #0
    ; Y: The digit value

    lda _souls_digits
    cmp #3
    beq show_h
    cmp #2
    beq show_t
    jmp show_u

    show_h:
        ldy nsk_bcd_outh
        _sprite_draw \
            { HUD::COUNTER::sprite_top, y       }, \
            { #HUD::COUNTER::PALETTE            }, \
            { #HUD::COUNTER::ATTRS              }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_TOP           }
        _sprite_draw \
            { HUD::COUNTER::sprite_bottom, y    }, \
            { #HUD::COUNTER::PALETTE            }, \
            { #HUD::COUNTER::ATTRS              }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_BOTTOM        }
        inx

    show_t:
        ldy nsk_bcd_outt
        _sprite_draw \
            { HUD::COUNTER::sprite_top, y       }, \
            { #HUD::COUNTER::PALETTE            }, \
            { #HUD::COUNTER::ATTRS              }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_TOP           }
        _sprite_draw \
            { HUD::COUNTER::sprite_bottom, y    }, \
            { #HUD::COUNTER::PALETTE            }, \
            { #HUD::COUNTER::ATTRS              }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_BOTTOM        }
        inx

    show_u:
        ldy nsk_bcd_outu
        _sprite_draw \
            { HUD::COUNTER::sprite_top, y       }, \
            { #HUD::COUNTER::PALETTE            }, \
            { #HUD::COUNTER::ATTRS              }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_TOP           }
        _sprite_draw \
            { HUD::COUNTER::sprite_bottom, y    }, \
            { #HUD::COUNTER::PALETTE            }, \
            { #HUD::COUNTER::ATTRS              }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_BOTTOM        }

    pull a, x, y

    rts
.endproc

; @brief Hide unused sprites
;
; @param _sprites_count
.proc _sprites_hide
    loop:
        lda _sprites_count
        cmp #NSK::PPU::SPRITES::MAXCOUNT
        beq done

        _sprite_hide

        jmp loop

    done:

    rts
.endproc

; @brief Draws fixed part of the HUD
.proc _sprites_draw_hud_fixed
    push y

    ldy #$00
    draw:
        _sprite_draw \
            {HUD::CAPTION::sprite, y}, \
            {HUD::CAPTION::palette,y}, \
            {#HUD::CAPTION::ATTRS   }, \
            {HUD::CAPTION::posx, y  }, \
            {HUD::CAPTION::posy, y  }

        iny
        cpy #HUD::CAPTION::COUNT
        bne draw

    pull y
    rts
.endproc

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

    loop:
        jai \
            _sprites_step_jumper,   \
            _sprites_steps,         \
            _sprites_step_index

        inc _sprites_step_index
        lda _sprites_step_index
        cmp #_SPRITES_STEPS_COUNT
        bne :+
            lda #$00
            sta _sprites_step_index
        :

        inc _sprites_step_counter
        lda _sprites_step_counter
        cmp #_SPRITES_STEPS_COUNT
        bne loop


    ; Hide the rest of the sprites (if any)
    jsr _sprites_hide

    ; Rotate starting step for the next frame
    inc _sprites_step_start
    lda _sprites_step_start
    cmp #_SPRITES_STEPS_COUNT
    bne :+
        lda #$00
        sta _sprites_step_start
    :

    pull a, x, y

    rts
.endproc

.export nsk_stage_sprites_draw_init
.proc nsk_stage_sprites_draw_init
    push a

    lda #$00
    sta _sprites_step_start

    pull a
    rts
.endproc

.endif