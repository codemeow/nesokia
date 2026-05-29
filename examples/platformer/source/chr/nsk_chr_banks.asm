; @file nsk_chr_banks.asm
; @brief Source module for the CHR ROM banks assignment
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_CHR_BANKS_ASM__
::__NSK_CHR_BANKS_ASM__ = 1

.segment "CHR0"

.incbin "chr/bank-0.0000.chr"
.incbin "chr/bank-0.1000.chr"

.segment "CHR1"

.incbin "chr/bank-1.0000.chr"
.incbin "chr/bank-1.1000.chr"

.segment "CHR2"

.incbin "chr/bank-2.0000.chr"
.incbin "chr/bank-2.1000.chr"

.segment "CHR3"

.incbin "chr/bank-3.0000.chr"
.incbin "chr/bank-3.1000.chr"

.endif
