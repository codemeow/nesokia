; @file nsk_sprites_common.inc
; @brief Include module for the sprites routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_COMMON__
::__NSK_SPRITES_COMMON__ = 1

.ifndef ::__NSK_SPRITES_COMMON_ASM__
; @brief Number of sprite slots occupied at the moment
.import _sprites_count

; @brief Sprite index for `nsk_sprite_draw`
.import _sprite_index
; @brief Sprite attribute byte for `nsk_sprite_draw`
.import _sprite_attr
; @brief Sprite posx for `nsk_sprite_draw`
.import _sprite_posx
; @brief Sprite posy for `nsk_sprite_draw`
.import _sprite_posy

; @brief Draws the sprite by stored values:
;
; @param _sprite_index Sprite index
; @param _sprite_attr  Sprite attribute byte
; @param _sprite_posx  Sprite X position
; @param _sprite_posy  Sprite Y position
; @param X             OAM shift in bytes
.import _sprite_draw_in

; @brief Hide unused sprites
;
; @param _sprites_count
.import nsk_sprites_hide

.endif

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
.macro nsk_sprite_draw index, palette, attributes, posx, posy
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

.endif