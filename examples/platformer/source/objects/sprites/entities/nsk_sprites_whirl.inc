; @file nsk_sprites_whirl.inc
; @brief Include module for the whirl sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_WHIRL__
::__NSK_SPRITES_WHIRL__ = 1

.ifndef ::__NSK_SPRITES_WHIRL_ASM__
    ; @brief Requested whirl X high byte
    .import nsk_whirl_query_x_hi

    ; @brief Requested whirl X low byte
    .import nsk_whirl_query_x_lo

    ; @brief Requests the current whirl position
    ;
    ; This will work only for the first whirl so if
    ; there will be more than one this should be reverted back
    ; to 06e69b142b6e198f8d17a81e06d05c8b4d9d0963
    ;
    ; @param[out] nsk_whirl_query_x_hi Hi X byte
    ; @param[out] nsk_whirl_query_x_lo Lo X byte
    ;
    ; This will also assume, that the whirl is 2x2 sprites
    .import nsk_whirl_requestx

    ; @brief Inits the whirl in the object pool
    .import nsk_whirl_init

    ; @brief Spawns one whirl in the object pool
    .import nsk_whirl_spawn

    ; @brief Routine to tick whirl-specific behavior
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_whirl_tick

    ; @brief Routine to clean up whirl object-specific data
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_whirl_clean

    ; @brief Routine to return the whirl collision box size
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_whirl_getbox

    ; @brief Routine to handle whirl collision
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    ; @param[in] nsk_pool_collision_other other collided pool index
    .import nsk_whirl_collision

    ; @brief Routine to draw the whirl
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_whirl_draw
.endif

.endif
