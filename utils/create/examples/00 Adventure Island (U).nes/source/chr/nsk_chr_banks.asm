; @file nsk_chr_banks.asm
; @brief Source module for the CHR ROM banks assignment
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_CHR_BANKS_ASM__
::__NSK_CHR_BANKS_ASM__ = 1

.segment "CHR0"
.incbin "../../chrrom/bank-0.chr"

.segment "CHR1"
.incbin "../../chrrom/bank-1.chr"

.segment "CHR2"
.incbin "../../chrrom/bank-2.chr"

.segment "CHR3"
.incbin "../../chrrom/bank-3.chr"

.endif