; @file nsk_sprites_list.inc
; @brief Include module for the sprite objects list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_LIST__
::__NSK_SPRITES_LIST__ = 1

; @brief List of sprite objects
.enum SPRITELIST
    HUD            ; @< HUD object
    CHARACTER      ; @< Main character
    METEORITE      ; @< Decorative falling meteorite
    WHIRL          ; @< Static whirl
    DEBRIS_0       ; @< Background debris, layer 0 (farthest)
    DEBRIS_1       ; @< Background debris, layer 1
    DEBRIS_2       ; @< Background debris, layer 2
    DEBRIS_3       ; @< Background debris, layer 3
;    FALLINGSTAR    ; @< Touched whirl
;    LYINGSTAR      ; @< Fallen whirl

    COUNT          ; @< Number of objects
.endenum

.include "entities/nsk_sprites_hud.inc"
.include "entities/nsk_sprites_character.inc"
.include "entities/nsk_sprites_meteorite.inc"
.include "entities/nsk_sprites_whirl.inc"
.include "entities/nsk_sprites_debris.inc"

.endif
