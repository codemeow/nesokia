; @file nsk_sprites_fallingstar.inc
; @brief Include module for the falling star sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_FALLINGSTAR__
::__NSK_SPRITES_FALLINGSTAR__ = 1

.ifndef ::__NSK_SPRITES_FALLINGSTAR_ASM__
    ; @brief Spawns one falling star at the source pool object coordinates
    ;
    ; @param[in] X source pool object index
    .import nsk_fallingstar_spawn

    ; @brief Routine to handle out-of-bounds falling star behavior
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_fallingstar_oob

    ; @brief Routine to tick falling star-specific behavior
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_fallingstar_tick

    ; @brief Routine to clean up falling star object-specific data
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_fallingstar_clean

    ; @brief Routine to check whether falling star touches ground
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    ; @param[out] nsk_pool_result 0 if empty, non-zero if solid
    .import nsk_fallingstar_isonground

    ; @brief Routine to return the falling star collision box size
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_fallingstar_getbox

    ; @brief Routine to handle falling star collision
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    ; @param[in] nsk_pool_collision_other other collided pool index
    .import nsk_fallingstar_collision

    ; @brief Routine to draw the falling star
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_fallingstar_draw
.endif

.endif
