; @file nsk_sprites_common.asm
; @brief Source module for the sprites routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_COMMON_ASM__
::__NSK_SPRITES_COMMON_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../../stage/handlers/sprites/nsk_sprites_common.inc"

.segment "BSS"

; @brief How many sprites are active in this frame
.export _sprites_count
_sprites_count:
    .res 1

; @brief Sprite index for `_sprite_draw`
.export _sprite_index
_sprite_index:
    .res 1

; @brief Sprite attribute byte for `_sprite_draw`
.export _sprite_attr
_sprite_attr:
    .res 1

; @brief Sprite posx for `_sprite_draw`
.export _sprite_posx
_sprite_posx:
    .res 1

; @brief Sprite posy for `_sprite_draw`
.export _sprite_posy
_sprite_posy:
    .res 1


.segment "CODE"

; @brief Hide unused sprites
;
; @param _sprites_count
.export nsk_sprites_hide
.proc nsk_sprites_hide
    push a, x

    loop:
        lda _sprites_count
        cmp #NSK::PPU::SPRITES::MAXCOUNT
        beq done

        asl
        asl
        tax

        lda #$ff
        sta NSK::CPU::RAM::OAMBUFFER, x

        inc _sprites_count

        jmp loop

    done:

    pull a, x

    rts
.endproc

; @brief Draws the sprite by stored values:
;
; @param _sprite_index Sprite index
; @param _sprite_attr  Sprite attribute byte
; @param _sprite_posx  Sprite X position
; @param _sprite_posy  Sprite Y position
; @param X             OAM shift in bytes
.export _sprite_draw_in
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

.endif