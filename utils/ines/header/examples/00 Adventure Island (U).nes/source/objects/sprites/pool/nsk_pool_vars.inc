; @file nsk_pool_vars.inc
; @brief Include module for the object pool variables
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_VARS__
::__NSK_POOL_VARS__ = 1

.include "nsk_pool_attrs.inc"
.include "nsk_pool_helpers.inc"

.ifndef ::__NSK_POOL_VARS_ASM__
    ; Data access
    _NSK_POOL_ATTRS _NSK_POOL_IMPORT_ONE

    ; @brief Number of occupied cells
    .import nsk_pool_size
.endif

.endif