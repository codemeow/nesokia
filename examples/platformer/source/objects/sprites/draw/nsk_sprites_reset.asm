; @file nsk_sprites_reset.asm
; @brief Source module for the sprites reset routine
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_RESET_ASM__
::__NSK_SPRITES_RESET_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_sprites_reset.inc"
.include "nsk_draw_vars.inc"

; @brief Reset the sprites counter
.export nsk_sprites_reset
.proc nsk_sprites_reset
    push a

    lda #0
    sta nsk_sprites_used

    pull a
    rts
.endproc

.endif
