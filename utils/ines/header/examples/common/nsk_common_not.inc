; @file nsk_common_not.inc
; @brief Include module for the inversion pseudo-instruction
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_COMMON_NOT__
::__NSK_COMMON_NOT__ = 1

; @brief Bitwise inversion of the A register
.macro not
    eor #$ff
.endmacro

.endif