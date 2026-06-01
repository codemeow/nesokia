; @file nsk_sprites_meteorite.inc
; @brief Include module for the meteorite sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_METEORITE__
::__NSK_SPRITES_METEORITE__ = 1

.ifndef ::__NSK_SPRITES_METEORITE_ASM__
    ; @brief Inits the meteorites in the object pool
    .import nsk_meteorite_init

    ; @brief Routine to handle out-of-bounds meteorite behavior
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_meteorite_oob

    ; @brief Routine to tick meteorite-specific behavior
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_meteorite_tick

    ; @brief Routine to clean up meteorite object-specific data
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_meteorite_clean

    ; @brief Routine to draw the meteorite
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_meteorite_draw
.endif

.endif
