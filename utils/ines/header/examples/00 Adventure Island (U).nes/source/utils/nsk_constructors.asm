; @file nsk_constructors.asm
; @brief Source module for constructor-like startup routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_CONSTRUCTORS_ASM__
::__NSK_CONSTRUCTORS_ASM__ = 1

.include "nsk_common_meta.inc"
.include "../utils/nsk_constructors.inc"

.import __CONSTRUCTORS_RUN__
.import __CONSTRUCTORS_SIZE__

.segment "CONSTRUCTORS"

; @brief Keeps the constructors segment defined even when no routines are registered
.addr $0000

.segment "BSS"

; @brief Jumper pointer
_nsk_constructors_jumper:
    .res 2

.segment "CODE"

; @brief Runs all registered constructor-like routines
.export nsk_constructors_run
.proc nsk_constructors_run
    push a, x, y

    ldx #0
    cpx #<__CONSTRUCTORS_SIZE__
    beq done

    loop:
        lda __CONSTRUCTORS_RUN__ + 0, x
        sta _nsk_constructors_jumper + 0
        lda __CONSTRUCTORS_RUN__ + 1, x
        sta _nsk_constructors_jumper + 1

        ; Failsafe: NULL pointers are ignored.
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
        cpx #<__CONSTRUCTORS_SIZE__
        bne loop

    done:
        pull a, x, y
        rts
.endproc

.endif
