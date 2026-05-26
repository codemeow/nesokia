; @file nsk_pool_vars.asm
; @brief Source module for the object pool variables
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_VARS_ASM__
::__NSK_POOL_VARS_ASM__ = 1

.include "nsk_pool_attrs.inc"
.include "nsk_pool_helpers.inc"

.segment "BSS"

_NSK_POOL_ATTRS _NSK_POOL_DEFINE_ONE
_NSK_POOL_ATTRS _NSK_POOL_EXPORT_ONE

; @brief Number of occupied cells
.export nsk_pool_size
nsk_pool_size:
    .res 1

; @brief Current X position of the current object on the screen
.export nsk_pool_screenx
nsk_pool_screenx:
    .res 1

; @brief Shared callback result scratch byte
.export nsk_pool_result
nsk_pool_result:
    .res 1

.endif
