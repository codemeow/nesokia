; @file nsk_stage_look_left.asm
; @brief Source module for the main loop camera moving
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_LOOK_LEFT_ASM__
::__NSK_STAGE_LOOK_LEFT_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../stage/handlers/nsk_stage_look_left.inc"
.include "../../stage/nsk_stage_list.inc"
.include "../../ppu/nsk_ppu_vars.inc"

.segment "CODE"

; @brief Changes the camera X position
.proc _change_x
    dec ppu_temp_scroll_x

    rts
.endproc

; @brief Stage for looking left
.export nsk_stage_look_left
.proc nsk_stage_look_left
    push a

    lda ppu_temp_scroll_x
    pha

    jsr _change_x

    pla
    cmp ppu_temp_scroll_x

    bcs :+
        lda #$00
        sta ppu_temp_scroll_x
        jsr nsk_stage_prev
    :

    pull a
    rts
.endproc

.endif