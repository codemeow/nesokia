; @file nsk_sprites_debris.inc
; @brief Include module for the debris sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_DEBRIS__
::__NSK_SPRITES_DEBRIS__ = 1

.ifndef ::__NSK_SPRITES_DEBRIS_ASM__
    ; @brief Routine to draw the debris
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_debris_draw
    ; @brief Routine to respawn the debris
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_debris_oob
    ; @brief Inits the debris in the object pool
    .import nsk_debris_init
.endif

.endif