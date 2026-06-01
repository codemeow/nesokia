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

    ; @brief Current X position of the current object on the screen
    .import nsk_pool_screenx

    ; @brief Shared callback result scratch byte
    .import nsk_pool_result

    ; @brief Shared collision box width result
    .import nsk_pool_box_width

    ; @brief Shared collision box height result
    .import nsk_pool_box_height

    ; @brief Other pool index for collision callbacks
    .import nsk_pool_collision_other
.endif

.endif
