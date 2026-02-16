; @file nsk_sprites_hud_fixed.asm
; @brief Source module for the sprites fixed part of the HUD drawing routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_HUD_FIXED_ASM__
::__NSK_SPRITES_HUD_FIXED_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "../../../stage/handlers/sprites/nsk_sprites_hud_fixed.inc"
.include "../../../stage/handlers/sprites/nsk_sprites_common.inc"

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
.endscope

.segment "CODE"

; @brief Draws fixed part of the HUD
.export nsk_sprites_hud_fixed
.proc nsk_sprites_hud_fixed
    push y

    ldy #$00
    draw:
        nsk_sprite_draw \
            {::HUD::CAPTION::sprite, y}, \
            {#::HUD::CAPTION::ATTRS   }, \
            {::HUD::CAPTION::palette,y}, \
            {::HUD::CAPTION::posx, y  }, \
            {::HUD::CAPTION::posy, y  }

        iny
        cpy #HUD::CAPTION::COUNT
        bne draw

    pull y
    rts
.endproc


; @brief Init module
.export nsk_sprites_hud_fixed_init
.proc nsk_sprites_hud_fixed_init
    rts
.endproc

.endif