; @file nsk_sprites_character.inc
; @brief Include module for the character sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_CHARACTER__
::__NSK_SPRITES_CHARACTER__ = 1

.ifndef ::__NSK_SPRITES_CHARACTER_ASM__
    ; @brief Inits the character in the object pool
    .import nsk_character_init

    ; @brief Routine to handle out-of-bounds character behavior
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_character_oob

    ; @brief Routine to tick character-specific behavior
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_character_tick

    ; @brief Routine to clean up character object-specific data
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_character_clean

    ; @brief Routine to check if the character stands on ground
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_character_isonground

    ; @brief Routine to draw the character
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_character_draw
.endif

.endif
