; @file nsk_sprites_starbit.inc
; @brief Include module for the star bit sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_STARBIT__
::__NSK_SPRITES_STARBIT__ = 1

.ifndef ::__NSK_SPRITES_STARBIT_ASM__
    ; @brief Spawns a burst of star bits at the source pool object coordinates
    ;
    ; @param[in] X source pool object index
    .import nsk_starbit_spawn_burst

    ; @brief Routine to tick star bit-specific behavior
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_starbit_tick

    ; @brief Routine to clean up star bit object-specific data
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_starbit_clean

    ; @brief Routine to check whether star bit touches ground
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    ; @param[out] nsk_pool_result 0 if empty, non-zero if solid
    .import nsk_starbit_isonground

    ; @brief Routine to draw the star bit
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_starbit_draw
.endif

.endif
