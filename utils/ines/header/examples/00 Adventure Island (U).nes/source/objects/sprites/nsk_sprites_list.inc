; @file nsk_sprites_list.inc
; @brief Include module for the sprite objects list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_LIST__
::__NSK_SPRITES_LIST__ = 1

; @brief List of sprite objects
.enum SPRITELIST
    HUD            ; @< HUD object
;    DEBRIS         ; @< Background debris
;    CHARACTER      ; @< Main character
;    WHIRL          ; @< Static whirl
;    FALLINGSTAR    ; @< Touched whirl
;    LYINGSTAR      ; @< Fallen whirl

    COUNT          ; @< Number of objects
.endenum

.include "entities/nsk_sprites_hud.inc"

.endif
