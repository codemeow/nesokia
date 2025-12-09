; @file nsk_stage_look_right.asm
; @brief Source module for the main loop camera moving
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_LOOK_RIGHT_ASM__
::__NSK_STAGE_LOOK_RIGHT_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../stage/handlers/nsk_stage_look_right.inc"
.include "../../stage/nsk_stage_list.inc"
.include "../../ppu/nsk_ppu_vars.inc"


.segment "CODE"

; @brief Changes the camera X position
.proc _change_x
    inc ppu_temp_scroll_x

    rts
.endproc

; @brief Stage for looking right
.export nsk_stage_look_right
.proc nsk_stage_look_right
    push a

    lda ppu_temp_scroll_x
    pha

    jsr _change_x

    pla
    cmp ppu_temp_scroll_x

    bcc :+
        lda #::NSK::SCREEN::WIDTH - 1
        sta ppu_temp_scroll_x
        jsr nsk_stage_next
    :

    pull a
    rts
.endproc

.endif