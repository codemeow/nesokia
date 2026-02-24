; @file nsk_pool_tick.asm
; @brief Source module for the object pool tick routine
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_TICK_ASM__
::__NSK_POOL_TICK_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_pool_tick.inc"

.include "nsk_pool_vars.inc"
.include "../draw/nsk_sprites_hide.inc"

.segment "BSS"

; Index of the object in the pool leads to the objects table,
; where the size/callbacks are located

; @brief Jump pointer
_table_draw_ptr:
    .res 2

.segment "CODE"

; @brief Object pool "tick" routine
; - Calculates the physics
; - Calls objects draw routines
.export nsk_pool_tick
.proc nsk_pool_tick
    push a, x, y

    ; Rotate pool elements here


;----- TEST

  ;  ldx #0
  ;  jai _table_draw_ptr, nsk_objects_table_draw, x

;----- TEST

    ; Hide the rest of the sprites (if any)
    nsk_sprites_hide

    pull a, x, y

    rts
.endproc

.endif