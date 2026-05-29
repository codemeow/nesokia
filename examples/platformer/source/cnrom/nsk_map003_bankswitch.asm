; @file nsk_map003_bankswitch.asm
; @brief Source module for the Mapper 003 bankswitching
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_MAP003_BANKSWITCH_ASM__
::__NSK_MAP003_BANKSWITCH_ASM__ = 1

.segment "BANKREG"

; @brief Locks the BANKREG segment to $ff value to resolve BUS conflicts
_nsk_map003_bankreg:
    .byte $ff

; @brief Changes the current CHR ROM bank to the value of A register
; @note The allowed values are [0..3]
.export nsk_map003_bankswitch
.proc nsk_map003_bankswitch
    sta _nsk_map003_bankreg
    rts
.endproc

.endif