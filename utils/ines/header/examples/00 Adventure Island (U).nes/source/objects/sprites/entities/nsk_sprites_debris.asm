; @file nsk_sprites_debris.asm
; @brief Source module for the debris sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_DEBRIS_ASM__
::__NSK_SPRITES_DEBRIS_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "nsk_sprites_debris.inc"
.include "../pool/nsk_pool_add.inc"
.include "../pool/nsk_pool_vars.inc"
.include "../nsk_sprites_list.inc"
.include "../../../utils/nsk_util_rand8.inc"
.include "../draw/nsk_sprites_draw.inc"

.segment "RODATA"

; @brief Debris settings
.scope DEBRIS
    OBJECT:
        .byte SPRITELIST::DEBRIS_0
        .byte SPRITELIST::DEBRIS_1
        .byte SPRITELIST::DEBRIS_2
        .byte SPRITELIST::DEBRIS_3

    ; @brief Sprites of the debris
    SPRITE:
        .byte $bf, $bf, $be, $be

    ; @brief Number of particles per layer
    SIZE:
        .byte 10, 8, 6, 4

    ; @brief Attributes per layer
    ATTRS:
        .byte NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::BACKGROUND
        .byte NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::BACKGROUND
        .byte NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::BACKGROUND
        .byte NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND

    ; @brief Palette per layer
    PALETTE:
        .byte %00, %00, %00, %00

    ; @brief Right-to-Left wind per layer (signed 8.8)
    WIND_LO:
        .byte $ff, $ff, $ff, $ff
    WIND_FRAC:
        .byte $90, $7e, $69, $21

    ; @brief Gravity per layer
    ;
    ; @note Not the engine's gravity is used, but the vector
    GRAVITY:
        .byte 10, 17, 31, 90

    ; @brief Number of layers
    LAYERS = 4

    ; @brief Object flags
    FLAGS = POOL::FLAGS::VECTORS
.endscope

.segment "BSS"

; @brief Temporary buffer for the X coord
_debris_worldx_hi:
    .res 1
; @brief Temporary buffer for the X coord
_debris_worldx_lo:
    .res 1
; @brief Temporary buffer for the Y coord
_debris_worldy_lo:
    .res 1
; @brief Temporary buffer for the object type


.segment "CODE"

; @brief Routine to draw the debris
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_debris_draw
.proc nsk_debris_draw
    push a, y

    ; Convert Object index to the DEBRIS tables index
    lda nsk_pool_object, x
    sec
    sbc #SPRITELIST::DEBRIS_0
    tay

    nsk_sprite_draw \
        { DEBRIS::SPRITE, y     }, \
        { DEBRIS::ATTRS, y      }, \
        { DEBRIS::PALETTE, y    }, \
        { nsk_pool_screenx      }, \
        { nsk_pool_worldy_lo, x }

    pull a, y

    rts
.endproc

; @brief Routine to respawn the debris
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_debris_oob
.proc nsk_debris_oob
    push a

    lda #1
    sta nsk_pool_worldx_hi, x

    lda #$ff
    sta nsk_pool_worldx_lo, x

    jsr nsk_util_rand8
    sta nsk_pool_worldy_lo, x

    pull a

    rts
.endproc


; @brief Inits the debris in the object pool
.export nsk_debris_init
.proc nsk_debris_init
    push a, x, y

    ; Layer index
    ldy #0

    layer:
        ; Debris index in the layer
        ldx #0

        particle:

            lda #0
            sta _debris_worldx_hi

            jsr nsk_util_rand8
            cmp #$7f
            bcs :+
                inc _debris_worldx_hi
            :

            jsr nsk_util_rand8
            sta _debris_worldx_lo

            jsr nsk_util_rand8
            sta _debris_worldy_lo

            nsk_pool_add \
                { DEBRIS::OBJECT, y  }, \
                { #DEBRIS::FLAGS     }, \
                { _debris_worldx_hi  }, \
                { _debris_worldx_lo  }, \
                { _debris_worldy_lo  }, \
                { DEBRIS::WIND_LO, y }, \
                { DEBRIS::WIND_FRAC, y }, \
                { #0                 }, \
                { DEBRIS::GRAVITY, y }, \
                { #0                 }

            inx
            txa
            cmp DEBRIS::SIZE,y
            bne particle

        iny
        cpy #DEBRIS::LAYERS
        bne layer

    pull a, x, y

    rts
.endproc

.endif
