; @file nsk_sprites_whirl.inc
; @brief Include module for the whirl sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_WHIRL__
::__NSK_SPRITES_WHIRL__ = 1

.ifndef ::__NSK_SPRITES_WHIRL_ASM__
    ; @brief Inits the whirl in the object pool
    .import nsk_whirl_init

    ; @brief Routine to tick whirl-specific behavior
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_whirl_tick

    ; @brief Routine to clean up whirl object-specific data
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_whirl_clean

    ; @brief Routine to draw the whirl
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_whirl_draw
.endif

.endif
