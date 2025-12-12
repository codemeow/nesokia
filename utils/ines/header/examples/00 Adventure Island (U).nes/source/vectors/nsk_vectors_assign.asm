; @file nsk_vectors_assign.asm
; @brief Source module for the VECTORS segment assigning
;
; Part of the Nesokia project — MIT License.
.ifndef ::__NSK_VECTORS_ASSIGN_ASM__
::__NSK_VECTORS_ASSIGN_ASM__ = 1

.include "../vectors/handlers/nsk_vector_nmi.inc"
.include "../vectors/handlers/nsk_vector_reset.inc"
.include "../vectors/handlers/nsk_vector_irq.inc"

.segment "VECTORS"

.addr nsk_vector_nmi
.addr nsk_vector_reset
.addr nsk_vector_irq

.endif