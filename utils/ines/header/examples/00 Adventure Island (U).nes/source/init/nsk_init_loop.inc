; @file nsk_init_loop.inc
; @brief Include module for the init functions loop runner
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_INIT_LOOP__
::__NSK_INIT_LOOP__ = 1

.include "nsk_common_meta.inc"

.ifndef ::__NSK_INIT_LOOP_ASM__
; @brief Address for the selected init functions list
.importzp _nsk_init_funcs
.endif

.ifndef ::__NSK_INIT_LOOP_ASM__
; @brief Loops over provided init functions
.import _nsk_init_loop
.endif

; @brief Loops over provided z-terminated init functions list
; @param [in] list List of functions
; ### Example
; ~~~
; funcs:
;    .addr func1
;    .addr func2
;    .addr $0000
; ...
; nsk_init_loop funcs
; ~~~
.macro nsk_init_loop list
    nsk_p16_set _nsk_init_funcs, #list, 16
    jsr _nsk_init_loop
.endmacro

.endif