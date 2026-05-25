; @file nsk_pool_tick.asm
; @brief Source module for the object pool tick routine
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_TICK_ASM__
::__NSK_POOL_TICK_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_pool_tick.inc"
.include "nsk_pool_remove.inc"
.include "nsk_pool_settings.inc"
.include "nsk_pool_vars.inc"
.include "../draw/nsk_sprites_hide.inc"
.include "../tables/nsk_sprites_tables.inc"
.include "../../../ppu/nsk_ppu_vars.inc"

.segment "BSS"

; Index of the object in the pool leads to the objects table,
; where the size/callbacks are located

; @brief Jump pointer
_table_ptr:
    .res 2

; @brief Current sweep index
_sweep_index:
    .res 1

.segment "CODE"

; @brief Ticks vector forces
;
; @param[in] X Current element index
.proc _pool_tick_vectors
    ; world_x += vector_x
    clc
    lda nsk_pool_worldx_frac, x
    adc nsk_pool_vectorx_frac, x
    sta nsk_pool_worldx_frac, x

    lda nsk_pool_worldx_lo, x
    adc nsk_pool_vectorx_lo, x
    sta nsk_pool_worldx_lo, x

    lda nsk_pool_worldx_hi, x
    adc #0
    tay
    lda nsk_pool_vectorx_lo, x
    bpl :+
        dey
    :
    tya
    sta nsk_pool_worldx_hi, x

    ; world_y += vector_y
    clc
    lda nsk_pool_worldy_frac, x
    adc nsk_pool_vectory_frac, x
    sta nsk_pool_worldy_frac, x

    lda nsk_pool_worldy_lo, x
    adc nsk_pool_vectory_lo, x
    sta nsk_pool_worldy_lo, x
    lda nsk_pool_vectory_lo, x
    bpl :+
        bcc oob
        jmp y_ok
    :
        bcs oob
    y_ok:

    ; Out-of-bounds checks
    lda nsk_pool_worldx_hi, x
    bmi oob
    cmp #2
    bcs oob

    ; Y is unsigned: bounds are handled by carry/borrow above

    rts

    oob:
        ldy nsk_pool_object, x
        jai _table_ptr, nsk_sprites_table_oob, y
        rts
.endproc

; @brief Ticks gravity-affected element
;
; @param[in] X Current element index
.proc _pool_tick_gravity

    nsk_todo "Gravity tick"

    rts
.endproc

; @brief Ticks collision-affected element
;
; @param[in] X Current element index
.proc _pool_tick_collision

    nsk_todo "Collision tick"

    rts
.endproc

; @brief Ticks one pool element
;
; @param[in] X Current element index
.proc _pool_tick_element
    ; FLAGS::FIXED skipped on purpose,
    ; there's nothing to do there

    lda nsk_pool_flags, x
    and #POOL::FLAGS::DELETED
    bne done

    lda nsk_pool_flags, x
    and #POOL::FLAGS::VECTORS
    beq :+
        jsr _pool_tick_vectors
    :

    lda nsk_pool_flags, x
    and #POOL::FLAGS::GRAVITY
    beq :+
        jsr _pool_tick_gravity
    :

    lda nsk_pool_flags, x
    and #POOL::FLAGS::COLLISION
    beq :+
        jsr _pool_tick_collision
    :

    done:
    rts
.endproc

; @brief Adds the element to the OAM list if it's visible
;
; @param[in] X Current element index
.proc _pool_draw_element
    push y

    ; Fixed objects are always visible
    lda nsk_pool_flags, x
    and #POOL::FLAGS::FIXED
    beq :+
        ; Also no saving screenx, as fixed objects
        ; use nsk_pool_worldx_lo directly
        jmp visible
    :

    ; screen_x = world_x_lo - scroll_x
    lda nsk_pool_worldx_lo, x
    sec
    sbc ppu_temp_scroll_x
    sta nsk_pool_screenx

    ; Carry flag meanings after SBC:
    ;   C = 1 : no borrow : world_x >= scroll_x
    ;   C = 0 : borrow    : world_x <  scroll_x

    lda nsk_pool_worldx_hi, x
    beq hi0

    ; hi != 0:
    ;   world_x is either far right of screen,
    ;   or slightly left but hi-byte was already non-zero
    hi1:
        ; No borrow : world_x > visible area (right)
        bcs not_visible
        ; Borrow: within visible area
        jmp visible

    ; hi == 0:
    ;   world_x is within or left of screen
    hi0:
        ; Borrow : world_x < scroll_x (left of screen)
        bcc not_visible
        ; No borrow: within visible area

    visible:
        ldy nsk_pool_object, x
        jai _table_ptr, nsk_sprites_table_draw, y

    not_visible:

    pull y

    rts
.endproc

; @brief Removes objects marked as deleted
.proc _pool_sweep_deleted
    push a, x

    ldx #0
    cpx nsk_pool_size
    bne loop
        jmp done

    loop:
        lda nsk_pool_flags, x
        and #POOL::FLAGS::DELETED
        beq next

            stx _sweep_index
            nsk_pool_remove _sweep_index

            ; Do not increment X: remove swaps the last object into
            ; the current slot, so the same index must be checked again.
            cpx nsk_pool_size
            beq done
            jmp loop

        next:
            inx
            cpx nsk_pool_size
            beq done
            jmp loop

    done:
        pull a, x
        rts
.endproc

; @brief Object pool "tick" routine
; - Calculates the physics
; - Calls objects draw routines
.export nsk_pool_tick
.proc nsk_pool_tick
    push a, x

    ldx #0
    cpx nsk_pool_size
    beq done


    loop:
        jsr _pool_tick_element

        lda nsk_pool_flags, x
        and #POOL::FLAGS::DELETED
        bne :+
            jsr _pool_draw_element
        :

        inx
        cpx nsk_pool_size
        bne loop


    done:

    nsk_todo "Rotate pool elements here"

    jsr _pool_sweep_deleted

    ; Hide the rest of the sprites (if any)
    nsk_sprites_hide

    pull a, x

    rts
.endproc

.endif
