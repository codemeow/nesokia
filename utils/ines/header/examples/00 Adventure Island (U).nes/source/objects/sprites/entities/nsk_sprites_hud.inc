; @file nsk_sprites_hud.inc
; @brief Include module for the HUD sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_HUD__
::__NSK_SPRITES_HUD__ = 1

.ifndef ::__NSK_SPRITES_HUD_ASM__
    ; @brief Routine to draw the HUD
    ;
    ; @param[in] X the index of the object in the nsk_pool_*
    .import nsk_hud_draw
    ; @brief Inits the HUD in the object pool
    .import nsk_hud_init
.endif

.endif