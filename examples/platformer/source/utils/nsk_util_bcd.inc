; @file nsk_util_bcd.inc
; @brief Include module for the BCD routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_UTIL_BCD__
::__NSK_UTIL_BCD__ = 1

.ifndef ::__NSK_UTIL_BCD_ASM__

.import nsk_bcd_outh ; Output "hundreds"
.import nsk_bcd_outt ; Output "tens"
.import nsk_bcd_outu ; Output "units"

; @brief ; @brief Bin to BCD 8 bits converter
; @param   a                Input register
; @param   nsk_bcd_outh     Output "hundreds" value
; @param   nsk_bcd_outt     Output "tens"     value
; @param   nsk_bcd_outu     Output "units"    value
.import nsk_bin8_todec
.endif

.endif