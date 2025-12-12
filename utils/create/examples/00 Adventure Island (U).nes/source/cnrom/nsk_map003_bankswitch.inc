; @file nsk_map003_bankswitch.inc
; @brief Include module for the Mapper 003 bankswitching
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_MAP003_BANKSWITCH__
::__NSK_MAP003_BANKSWITCH__ = 1

.ifndef ::__NSK_MAP003_BANKSWITCH_ASM__
; @brief Changes the current CHR ROM bank to the value of A register
; @note The allowed values are [0..3]
.import nsk_map003_bankswitch
.endif

; @brief Number of CHR ROM banks in Mapper 003
NSK_MAP003_BANKCOUNT = 4

.endif