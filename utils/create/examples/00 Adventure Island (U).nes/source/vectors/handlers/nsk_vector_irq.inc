; @file nsk_vector_irq.inc
; @brief Include module for the IRQ handler vector
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_VECTOR_IRQ__
::__NSK_VECTOR_IRQ__ = 1

.ifndef ::__NSK_VECTOR_IRQ_ASM__
; @brief IRQ handler routine
.import nsk_vector_irq
.endif

.endif