; @file nsk_reset_constructors.asm
; @brief Source module for constructor-like startup routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_RESET_CONSTRUCTORS_ASM__
::__NSK_RESET_CONSTRUCTORS_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"
.include "../../nsk_frame_configs.inc"

.if .defined(::NSK_FEATURE_CONSTRUCTORS) \
    .and ::NSK_FEATURE_CONSTRUCTORS = 1

.define _NSK_CONSTRUCTORS_RUN \
    .ident(.sprintf("__%s_RUN__", NSK_SEGMENT_CONSTRUCTORS))
.define _NSK_CONSTRUCTORS_SIZE \
    .ident(.sprintf("__%s_SIZE__", NSK_SEGMENT_CONSTRUCTORS))

.import _NSK_CONSTRUCTORS_RUN
.import _NSK_CONSTRUCTORS_SIZE

.assert _NSK_CONSTRUCTORS_SIZE < 256, \
    error, \
    .sprintf("%s segment should be less than 256 bytes (~128 functions)", NSK_SEGMENT_CONSTRUCTORS)

.segment NSK_SEGMENT_CONSTRUCTORS

; Keeps the constructors segment in place
.addr $0000

.segment NSK_SEGMENT_BSS

_nsk_constructors_jumper:
    .res 2

.segment NSK_SEGMENT_CONSTRUCTORSRUNCODE

; @brief Runs all registered constructor-like routines
;
; Manually declare .import in need of manual initialization
.export nsk_constructors_run
.proc nsk_constructors_run
    push a, x, y

    ldx #0
    cpx #<_NSK_CONSTRUCTORS_SIZE
    beq done

    loop:
        lda _NSK_CONSTRUCTORS_RUN + 0, x
        sta _nsk_constructors_jumper + 0
        lda _NSK_CONSTRUCTORS_RUN + 1, x
        sta _nsk_constructors_jumper + 1

        ; Failsafe
        ora _nsk_constructors_jumper + 0
        beq next

        push x

        lda #>(resume - 1)
        pha
        lda #<(resume - 1)
        pha

        jmp (_nsk_constructors_jumper)

        resume:
            pull x

        next:
            inx
            inx
            cpx #<_NSK_CONSTRUCTORS_SIZE
            bne loop

    done:
        pull a, x, y

        rts
.endproc

.endif

.endif