; @file nsk_vector_irq.asm
; @brief Source module for the IRQ handler vector
;
; Part of the Nesokia project — MIT License.
.ifndef ::__NSK_VECTOR_IRQ_ASM__
::__NSK_VECTOR_IRQ_ASM__ = 1

.include "../../vectors/handlers/nsk_vector_irq.inc"

.segment "CODE"

; @brief IRQ handler routine
.export nsk_vector_irq
.proc nsk_vector_irq
    rti
.endproc

.endif