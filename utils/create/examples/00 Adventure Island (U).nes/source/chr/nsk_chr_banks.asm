; @file nsk_chr_banks.asm
; @brief Source module for the CHR ROM banks assignment
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_CHR_BANKS_ASM__
::__NSK_CHR_BANKS_ASM__ = 1

.segment "CHR0"

.incbin "../../chrrom/chr/bank-0.0.chr"
.incbin "../../chrrom/chr/bank-0.1.chr"

.segment "CHR1"

.incbin "../../chrrom/chr/bank-1.0.chr"
.incbin "../../chrrom/chr/bank-1.1.chr"

.segment "CHR2"

.incbin "../../chrrom/chr/bank-2.0.chr"
.incbin "../../chrrom/chr/bank-2.1.chr"

.segment "CHR3"

.incbin "../../chrrom/chr/bank-3.0.chr"
.incbin "../../chrrom/chr/bank-3.1.chr"

.endif