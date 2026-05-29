; @file nsk_common_constructor.inc
; @brief Include module for constructor-like startup routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_COMMON_CONSTRUCTOR__
::__NSK_COMMON_CONSTRUCTOR__ = 1

; @brief Registers a constructor-like routine
;
; @param[in] routine Routine address. The routine must end with RTS.
.macro nsk_constructor routine
    .pushseg
    .segment "CONSTRUCTORS"
        .addr routine
    .popseg
.endmacro

.endif