; @file nsk_vector_irq.asm
; @brief Source module for the IRQ handler vector
;
; Part of the Nesokia project — MIT License.
.ifndef ::__NSK_VECTOR_IRQ_ASM__
::__NSK_VECTOR_IRQ_ASM__ = 1

.include "nsk_common_meta.inc"
.include "../../vectors/handlers/nsk_vector_irq.inc"

.segment "CODE"

; @brief IRQ handler routine
.export nsk_vector_irq
.proc nsk_vector_irq
    nsk_todo "If defined ::NSK_DEBUG - do runtime assert"

    ; Detect whether this handler was entered via BRK or hardware IRQ.
    ; The CPU pushes P onto the stack on interrupt entry:
    ;   IRQ => B flag cleared
    ;   BRK => B flag set
    ;
    ; After our register pushes in this fuction, saved P is located at $0100+SP+offset.
    ; Example:
    ;   tsx
    ;   lda $0104,x    ; adjust offset to match prologue layout
    ;   and #$10       ; test B flag

    rti
.endproc

.endif