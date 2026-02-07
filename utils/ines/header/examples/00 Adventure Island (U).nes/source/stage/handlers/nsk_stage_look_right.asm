; @file nsk_stage_look_right.asm
; @brief Source module for the main loop camera moving
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_LOOK_RIGHT_ASM__
::__NSK_STAGE_LOOK_RIGHT_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../ppu/nsk_ppu_vars.inc"
.include "../../stage/handlers/nsk_stage_look_right.inc"
.include "../../stage/handlers/nsk_stage_sprites.inc"
.include "../../stage/nsk_stage_list.inc"

; Module settings
.scope NSK_LOCAL_SETTINGS
    ; How many frames should pass before scrolling
    FRAMES_PER_PIXEL = 3
.endscope

.segment "BSS"

; Frames counter
scroll_frames:
    .res 1

.segment "CODE"

; @brief Changes the camera X position
.proc _change_x
    push a

    inc scroll_frames
    lda scroll_frames
    cmp #NSK_LOCAL_SETTINGS::FRAMES_PER_PIXEL
    bne done

        lda #$00
        sta scroll_frames
        inc ppu_temp_scroll_x

    done:

    pull a
    rts


;    inc ppu_temp_scroll_x

;    rts
.endproc

; @brief Move camera to the right
.proc _look_right
    push a

    lda ppu_temp_scroll_x
    pha

    jsr _change_x

    pla
    cmp ppu_temp_scroll_x

    beq :+
    bcc :+
        lda #::NSK::SCREEN::WIDTH - 1
        sta ppu_temp_scroll_x
        jsr nsk_stage_next
    :

    pull a
    rts
.endproc

; @brief Stage for looking right
.export nsk_stage_look_right
.proc nsk_stage_look_right
    jsr _look_right
    jsr nsk_stage_sprites_draw
    rts
.endproc

; @brief Init this stage
.export nsk_stage_look_right_init
.proc nsk_stage_look_right_init
    push a

    lda #$00
    sta scroll_frames

    pull a
    rts
.endproc

.endif