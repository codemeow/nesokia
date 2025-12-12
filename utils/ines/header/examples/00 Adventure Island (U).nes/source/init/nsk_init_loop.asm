; @file nsk_init_loop.asm
; @brief Source module for the init functions loop runner
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_INIT_LOOP_ASM__
::__NSK_INIT_LOOP_ASM__ = 1

.include "../init/nsk_init_loop.inc"

.segment "ZEROPAGE"

; @brief Address for the selected init functions list
.exportzp _nsk_init_funcs
_nsk_init_funcs:
    .res 2

; @brief Pointer for the init function address for the indirect jump
nsk_init_jumper:
    .res 2

.segment "CODE"

; @brief "Jumper"-helper to avoid manual address setting
_nsk_init_jump:
    jmp (nsk_init_jumper)

; @brief Loops over provided init functions
.export _nsk_init_loop
.proc _nsk_init_loop
    push a, y

    ldy #$00

    loop:
        lda (_nsk_init_funcs), y
        sta nsk_init_jumper + 0
        iny
        lda (_nsk_init_funcs), y
        sta nsk_init_jumper + 1
        iny

        lda nsk_init_jumper + 0
        ora nsk_init_jumper + 1
        beq done

        jsr _nsk_init_jump
        jmp loop
    done:

    pull a, y

    rts

    rts
.endproc

.endif