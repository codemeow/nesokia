; @file nsk_sprites_draw.inc
; @brief Include module for the sprites draw routine
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_DRAW__
::__NSK_SPRITES_DRAW__ = 1

.include "nsk_common_meta.inc"

.include "nsk_draw_vars.inc"

; @brief Hides the unused sprites from screen
.macro nsk_sprites_hide
    .local loop
    .local done

    push a,x,y

    lda nsk_sprites_used
    cmp #NSK::PPU::SPRITES::MAXCOUNT
    beq done

    asl
    asl
    tax

    lda #NSK::PPU::SPRITES::MAXCOUNT
    sec
    sbc nsk_sprites_used
    tay

    lda #$ff

    loop:
        sta NSK::CPU::RAM::OAMBUFFER, x

        inx
        inx
        inx
        inx

        dey
        bne loop

    done:

    pull a,x,y
.endmacro

.endif