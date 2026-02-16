; @file nsk_sprites_hud_counter.asm
; @brief Source module for the sprites counter part of the HUD drawing routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_HUD_COUNTER_ASM__
::__NSK_SPRITES_HUD_COUNTER_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../../stage/handlers/sprites/nsk_sprites_hud_counter.inc"
.include "../../../stage/handlers/sprites/nsk_sprites_common.inc"
.include "../../../utils/nsk_util_bcd.inc"
.include "../../../ppu/nsk_ppu_vars.inc"

.segment "RODATA"

;@brief HUD-related settings
;
; @note The actual Y position should be 1 pixel less, but in the purpose of demo
;       and readablity I keep the original values
.scope HUD
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

.segment "BSS"

; @brief Current number of "souls"
_hud_souls:
    .res 1

; @brief Number of digits in the "souls" counter as decimal
_souls_digits:
    .res 1

.segment "CODE"

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

; @brief Draws the counter part of the HUD
.export nsk_sprites_hud_counter
.proc nsk_sprites_hud_counter
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
        nsk_sprite_draw \
            { HUD::COUNTER::sprite_top, y       }, \
            { #HUD::COUNTER::ATTRS              }, \
            { #HUD::COUNTER::PALETTE            }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_TOP           }
        nsk_sprite_draw \
            { HUD::COUNTER::sprite_bottom, y    }, \
            { #HUD::COUNTER::ATTRS              }, \
            { #HUD::COUNTER::PALETTE            }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_BOTTOM        }
        inx

    show_t:
        ldy nsk_bcd_outt
        nsk_sprite_draw \
            { HUD::COUNTER::sprite_top, y       }, \
            { #HUD::COUNTER::ATTRS              }, \
            { #HUD::COUNTER::PALETTE            }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_TOP           }
        nsk_sprite_draw \
            { HUD::COUNTER::sprite_bottom, y    }, \
            { #HUD::COUNTER::ATTRS              }, \
            { #HUD::COUNTER::PALETTE            }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_BOTTOM        }
        inx

    show_u:
        ldy nsk_bcd_outu
        nsk_sprite_draw \
            { HUD::COUNTER::sprite_top, y       }, \
            { #HUD::COUNTER::ATTRS              }, \
            { #HUD::COUNTER::PALETTE            }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_TOP           }
        nsk_sprite_draw \
            { HUD::COUNTER::sprite_bottom, y    }, \
            { #HUD::COUNTER::ATTRS              }, \
            { #HUD::COUNTER::PALETTE            }, \
            { HUD::COUNTER::posx, x             }, \
            { #HUD::COUNTER::POSY_BOTTOM        }

    pull a, x, y

    rts
.endproc

; @brief Init module
.export nsk_sprites_hud_counter_init
.proc nsk_sprites_hud_counter_init
    rts
.endproc

.endif