; @file nsk_main.asm
; @brief Source module for the main routine
;
; Part of the Nesokia project — MIT License.
.ifndef ::__NSK_MAIN_ASM__
::__NSK_MAIN_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "nsk_main.inc"
.include "stage/nsk_stage_list.inc"
.include "vectors/handlers/nsk_vector_nmi.inc"

.segment "CODE"

; @brief Main loop function
.export nsk_main
.proc nsk_main
    jsr nsk_stage_run

    loop:
        ; <code>

    inc nsk_nmi_sleep_flag
    sleep:
        lda nsk_nmi_sleep_flag
        bne sleep

    jmp loop

.endproc

.endif