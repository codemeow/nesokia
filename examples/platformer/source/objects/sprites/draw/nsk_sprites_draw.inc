; @file nsk_sprites_draw.inc
; @brief Include module for the sprites draw routine
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_DRAW__
::__NSK_SPRITES_DRAW__ = 1

.linecont +

.include "nsk_draw_helpers.inc"
.include "nsk_draw_vars.inc"

; @brief Draws one single sprite
; @param[in] index      Sprite index
; @param[in] attributes Sprite attributes ([XXXX XX00])
; @param[in] palette    Sprite palette    ([0000 00XX])
; @param[in] posx       Sprite X position
; @param[in] posy       Sprite Y position
;
; @param[in] _SPRITES_COUNT Number of used sprites
;
; @note Checks on _SPRITES_COUNT. If it's equal to _SPRITES_MAX, exits instead
; @note Clobbers A
.macro nsk_sprite_draw index, attributes, palette, posx, posy
    .local done

    ; Do not write outside of the limit
    lda _SD_SPRITES_COUNT
    cmp #_SD_SPRITES_MAX
    beq done

    argtype0  .set _SD_ARG_TYPE_NONINDEXED ;@< PosY OAM byte addressing type
    argtype1  .set _SD_ARG_TYPE_NONINDEXED ;@< Index OAM byte addressing type
    argtype2a .set _SD_ARG_TYPE_NONINDEXED ;@< Attribute OAM byte part addressing type
    argtype2b .set _SD_ARG_TYPE_NONINDEXED ;@< Palette OAM byte part addressing type
    argtype3  .set _SD_ARG_TYPE_NONINDEXED ;@< PosX OAM byte addressing type

    _SD_ARG_CHECK { posy       }, argtype0
    _SD_ARG_CHECK { index      }, argtype1
    _SD_ARG_CHECK { attributes }, argtype2a
    _SD_ARG_CHECK { palette    }, argtype2b
    _SD_ARG_CHECK { posx       }, argtype3

    _SD_X_PUSH
    _SD_Y_PUSH       argtype0, argtype1, argtype2a, argtype2b, argtype3

    _SD_PLAN_BUILD   argtype0, argtype1, argtype2a, argtype2b, argtype3

    _SD_OAMADDR_CALC

    _SD_PLAN_WRITE   \
        { posy       }, argtype0,  \
        { index      }, argtype1,  \
        { attributes }, argtype2a, \
        { palette    }, argtype2b, \
        { posx       }, argtype3

    inc _SD_SPRITES_COUNT

    _SD_Y_PULL argtype0, argtype1, argtype2a, argtype2b, argtype3
    _SD_X_PULL

    done:
.endmacro

.endif