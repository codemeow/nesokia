; @file nsk_header_log2.inc
; @brief Include module for the log2 compile-time calculation
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_LOG2__
::__NSK_HEADER_LOG2__ = 1

; @macro nsk_header_log2
; @brief Compute the integer base-2 logarithm of a constant value at compile-
;        time.
;
; @param value       Constant integer to check (must be > 0 and a power of two).
; @param out_result  Symbol assigned to the resulting exponent n.
;
; @error Emits an error if @p value = 0 or not a power of two.
.macro nsk_header_log2 value, out_result
.scope
    check:
        .if value = 0 || (value & (value - 1)) <> 0
            .error "Invalid value passed to the `nsk_header_log2`"
        .endif

    init:
        _found .set 0

    exponent_loop:
        .repeat 32, _loop_exponent
            .if _found = 0
                .if 1 << _loop_exponent = value
                    out_result .set _loop_exponent
                    _found .set 1
                .endif
            .endif
        .endrepeat

    failcheck:
        .if _found = 0
            .error .sprintf("Failed to find log2(%d)", value)
        .endif
.endscope
.endmacro

.endif
