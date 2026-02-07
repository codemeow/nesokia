; @file nsk_stage_look_left.asm
; @brief Source module for the main loop camera moving
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_LOOK_LEFT_ASM__
::__NSK_STAGE_LOOK_LEFT_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../ppu/nsk_ppu_vars.inc"
.include "../../stage/handlers/nsk_stage_look_left.inc"
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
        dec ppu_temp_scroll_x

    done:

    pull a
    rts
.endproc

; @brief Move camera window
.proc _look_left
    push a

    lda ppu_temp_scroll_x
    pha

    jsr _change_x

    pla
    cmp ppu_temp_scroll_x

    beq :+
    bcs :+
        lda #$00
        sta ppu_temp_scroll_x
        jsr nsk_stage_prev
    :

    pull a
    rts
.endproc

; @brief Stage for looking left
.export nsk_stage_look_left
.proc nsk_stage_look_left
    jsr _look_left
    jsr nsk_stage_sprites_draw
    rts
.endproc

; @brief Init this stage
.export nsk_stage_look_left_init
.proc nsk_stage_look_left_init
    push a

    lda #$00
    sta scroll_frames

    pull a
    rts
.endproc

.endif