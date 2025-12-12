; @file nsk_common_debug.inc
; @brief Include module for the debug output commands
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_COMMON_DEBUG__
::__NSK_COMMON_DEBUG__ = 1

.feature string_escapes

; @brief Prints TODO-element
; @param [in] text To-do element to print
.macro nsk_todo text
    .out .sprintf("\x1b[1;33m - TODO:\x1b[0m %s", text)
.endmacro

; @brief Prints DEBUG-element
; @param [in] text To-do element to print
.macro nsk_debug text
    .out .sprintf("\x1b[1;34m - DEBUG:\x1b[0m %s", text)
.endmacro

.endif
