; @file nsk_sprites_hud.asm
; @brief Source module for the HUD sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_HUD_ASM__
::__NSK_SPRITES_HUD_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "nsk_sprites_hud.inc"

.include "../draw/nsk_sprites_draw.inc"
.include "../pool/nsk_pool_vars.inc"
.include "../pool/nsk_pool_add.inc"
.include "../pool/nsk_pool_attrs.inc"

.include "../nsk_sprites_list.inc"

.segment "RODATA"

;@brief HUD-related settings
;
; @note The actual Y position should be 1 pixel less, but in the purpose of demo
;       and readablity I keep the original values
.scope HUD
    .scope CAPTION
        ; Screen start coordinates
        SCREENX = 16
        SCREENY = 24

        SPRITE:
            .byte $13,$14,$15
        POSX:
            .byte 0,8,16
        POSY:
            .byte 0,0,0

        ; Same attributes for all sprites in purpose of simplicity
        ATTRS = \
            NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
            NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::NO        | \
            NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO
        ; Same palette for all digits
        PALETTE = %00

        COUNT = 3   ; Number of fixed HUD sprites
    .endscope

    .scope COUNTER
        ; Screen start coordinates
        SCREENX = 48
        SCREENY = 16

        POSX:
            .byte 0,8,16,24
        POSY_TOP    = 0 ; Top row Y position
        POSY_BOTTOM = 8 ; Bottom row Y position

        SPRITE_TOP:
            .byte $06,$07,$08,$09,$0a, $0b,$0c,$0d,$0e,$0f
        SPRITE_BOTTOM:
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

; @brief Screen coordinates X
_hud_screenx:
    .res 1
; @brief Screen coordinates Y
_hud_screeny:
    .res 1

.segment "CODE"

; @brief Routine to draw the fixed part of the HUD
;
; @param[in] X the index of the object in the nsk_stack_*
.proc _hud_fixed_draw
    push a, y

    ldy #0

    loop:
        clc
        lda nsk_pool_worldy_lo, x
        adc HUD::CAPTION::POSY, y
        sta _hud_screeny

        clc
        lda nsk_pool_worldx_lo, x
        adc HUD::CAPTION::POSX, y
        sta _hud_screenx

        nsk_sprite_draw \
            { HUD::CAPTION::SPRITE, y }, \
            { #HUD::CAPTION::ATTRS    }, \
            { #HUD::CAPTION::PALETTE  }, \
            { _hud_screenx            }, \
            { _hud_screeny            }

        iny
        cpy #HUD::CAPTION::COUNT
        bne loop

    pull a, y

    rts
.endproc


; @brief Routine to draw the counter part of the HUD
;
; @param[in] X the index of the object in the nsk_stack_*
.proc _hud_counter_draw

    rts
.endproc

; @brief Routine to draw the HUD
.export nsk_hud_draw
.proc nsk_hud_draw
    jsr _hud_fixed_draw
    jsr _hud_counter_draw
    rts
.endproc

; @brief Inits the HUD in the object pool
.export nsk_hud_init
.proc nsk_hud_init
    nsk_pool_add \
        { #SPRITELIST::HUD          }, \
        { #POOL::FLAGS::FIXED       }, \
        { #0                        }, \
        { #HUD::CAPTION::SCREENX    }, \
        { #HUD::CAPTION::SCREENY    }, \
        { #0                        }, \
        { #0                        }, \
        { #0                        }
    rts
.endproc

.endif