; @file nsk_sprites_common.asm
; @brief Source module for the sprites routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_COMMON_ASM__
::__NSK_SPRITES_COMMON_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../../stage/handlers/sprites/nsk_sprites_common.inc"

.segment "BSS"

; @brief How many sprites are active in this frame
.export _sprites_count
_sprites_count:
    .res 1

.endif