; @file nsk_constructors.inc
; @brief Include module for constructor-like startup routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_CONSTRUCTORS__
::__NSK_CONSTRUCTORS__ = 1

; nsk_constructor requirement
.define NSK_SEGMENT_CONSTRUCTORS "CONSTRUCTORS"

.ifndef ::__NSK_CONSTRUCTORS_ASM__
; @brief Runs all registered constructor-like routines
.import nsk_constructors_run
.endif

.endif
