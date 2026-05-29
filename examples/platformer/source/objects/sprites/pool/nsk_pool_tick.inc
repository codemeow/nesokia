; @file nsk_pool_tick.inc
; @brief Include module for the object pool tick routine
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_TICK__
::__NSK_POOL_TICK__ = 1

.ifndef ::__NSK_POOL_TICK_ASM__
    ; @brief Object pool "tick" routine
    ; - Calculates the physics
    ; - Calls objects draw routines
    .import nsk_pool_tick
.endif

.endif