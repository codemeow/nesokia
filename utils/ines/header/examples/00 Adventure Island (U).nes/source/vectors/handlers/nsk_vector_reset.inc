; @file nsk_vector_reset.inc
; @brief Include module for the reset vector
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_VECTOR_RESET__
::__NSK_VECTOR_RESET__ = 1

.ifndef ::__NSK_VECTOR_RESET_ASM__
; @brief Reset handler routine
; @note Never returns
.import nsk_vector_reset
.endif

.endif