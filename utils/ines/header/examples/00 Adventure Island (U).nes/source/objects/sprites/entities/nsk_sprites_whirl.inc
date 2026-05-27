; @file nsk_sprites_whirl.inc
; @brief Include module for the whirl sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_WHIRL__
::__NSK_SPRITES_WHIRL__ = 1

.ifndef ::__NSK_SPRITES_WHIRL_ASM__
    ; @brief Query X high byte for nsk_whirl_nearest_find
    .import nsk_whirl_query_x_hi

    ; @brief Query X low byte for nsk_whirl_nearest_find
    .import nsk_whirl_query_x_lo

    ; @brief Query direction for nsk_whirl_nearest_find (LEFT=0, RIGHT=1)
    .import nsk_whirl_query_direction

    ; @brief Finds the nearest whirl in the query direction
    ;
    ; @param[in] nsk_whirl_query_x_hi        Query X high byte
    ; @param[in] nsk_whirl_query_x_lo        Query X low byte
    ; @param[in] nsk_whirl_query_direction  LEFT=0, RIGHT=1
    ; @param[out] X current pool index of the nearest whirl, or $ff
    .import nsk_whirl_nearest_find

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
