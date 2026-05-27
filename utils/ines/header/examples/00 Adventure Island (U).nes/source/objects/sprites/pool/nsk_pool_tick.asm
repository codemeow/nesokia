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

; @brief Current pool size copy for safe iteration for the
; cases when the pool size is manipulated inside of the pool tick
_pool_size:
    .res 1

; @brief First pool index used by the rotated draw pass
_pool_draw_start:
    .res 1

; @brief Current pool index used by the rotated draw pass
_pool_draw_index:
    .res 1

; @brief Number of objects left to process in the draw pass
_pool_draw_count:
    .res 1
; @brief First collision pair pool index
_collision_a_index:
    .res 1
; @brief Second collision pair pool index
_collision_b_index:
    .res 1
; @brief First collision pair box width
_collision_a_width:
    .res 1
; @brief First collision pair box height
_collision_a_height:
    .res 1
; @brief Second collision pair box width
_collision_b_width:
    .res 1
; @brief Second collision pair box height
_collision_b_height:
    .res 1
; @brief First collision pair right X low byte
_collision_a_right_lo:
    .res 1
; @brief First collision pair right X high byte
_collision_a_right_hi:
    .res 1
; @brief Second collision pair right X low byte
_collision_b_right_lo:
    .res 1
; @brief Second collision pair right X high byte
_collision_b_right_hi:
    .res 1
; @brief First collision pair bottom Y
_collision_a_bottom:
    .res 1
; @brief Second collision pair bottom Y
_collision_b_bottom:
    .res 1

.segment "CODE"

; @brief Ticks object-specific behavior
;
; @param[in] X Current element index
.proc _pool_tick_object
    jaic _table_ptr, nsk_sprites_table_tick, { nsk_pool_object, x}

    rts
.endproc

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
        jaic _table_ptr, nsk_sprites_table_oob, { nsk_pool_object, x }
        rts
.endproc

; @brief Ticks gravity-affected element
;
; @param[in] X Current element index
.proc _pool_tick_gravity

    ; Negative Y velocity means the object is moving upward. Do not ask the
    ; object whether it is on the ground yet, otherwise a jump started while
    ; standing on ground is cancelled before vectors can move the object.
    lda nsk_pool_vectory_lo, x
    bmi falling

    lda #0
    sta nsk_pool_result

    jaic _table_ptr, nsk_sprites_table_isonground, { nsk_pool_object, x }

    lda nsk_pool_result
    beq falling

    lda #0
    sta nsk_pool_vectory_lo, x
    sta nsk_pool_vectory_frac, x
    jmp done

    falling:
        clc
        lda nsk_pool_vectory_frac, x
        adc #POOL::GRAVITY::ACCEL_FRAC
        sta nsk_pool_vectory_frac, x

        lda nsk_pool_vectory_lo, x
        adc #POOL::GRAVITY::ACCEL_LO
        sta nsk_pool_vectory_lo, x

        ; Clamp only positive falling velocity. Negative velocity is an
        ; upward movement and gravity should bring it back down naturally.
        bmi done

        cmp #POOL::GRAVITY::MAX_LO
        bcc done
        bne clamp

        lda nsk_pool_vectory_frac, x
        cmp #POOL::GRAVITY::MAX_FRAC
        bcc done

    clamp:
        lda #POOL::GRAVITY::MAX_LO
        sta nsk_pool_vectory_lo, x
        lda #POOL::GRAVITY::MAX_FRAC
        sta nsk_pool_vectory_frac, x

    done:

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

    jsr _pool_tick_object

    lda nsk_pool_flags, x
    and #POOL::FLAGS::GRAVITY
    beq :+
        jsr _pool_tick_gravity
    :

    lda nsk_pool_flags, x
    and #POOL::FLAGS::VECTORS
    beq :+
        jsr _pool_tick_vectors
    :

    ; FLAGS::COLLISION is skipped on purpose. Pair collisions are handled
    ; after all objects finish their regular tick/vector/gravity updates.

    done:
    rts
.endproc

; @brief Checks whether the current collision pair can collide
;
; @param[in] X Pool index
; @param[out] A zero if this object cannot collide
.proc _pool_collision_flags_check
    lda nsk_pool_flags, x
    and #POOL::FLAGS::DELETED
    bne no

    lda nsk_pool_flags, x
    and #POOL::FLAGS::COLLISION
    rts

    no:
        lda #0
        rts
.endproc

; @brief Calculates the current collision pair boxes
.proc _pool_collision_boxes_calc
    ldx _collision_a_index
    lda #0
    sta nsk_pool_box_width
    sta nsk_pool_box_height
    jaic _table_ptr, nsk_sprites_table_getbox, { nsk_pool_object, x }
    lda nsk_pool_box_width
    sta _collision_a_width
    lda nsk_pool_box_height
    sta _collision_a_height

    ldx _collision_b_index
    lda #0
    sta nsk_pool_box_width
    sta nsk_pool_box_height
    jaic _table_ptr, nsk_sprites_table_getbox, { nsk_pool_object, x }
    lda nsk_pool_box_width
    sta _collision_b_width
    lda nsk_pool_box_height
    sta _collision_b_height

    ldx _collision_a_index
    clc
    lda nsk_pool_worldx_lo, x
    adc _collision_a_width
    sta _collision_a_right_lo
    lda nsk_pool_worldx_hi, x
    adc #0
    sta _collision_a_right_hi

    clc
    lda nsk_pool_worldy_lo, x
    adc _collision_a_height
    sta _collision_a_bottom
    bcc :+
        lda #$ff
        sta _collision_a_bottom
    :

    ldx _collision_b_index
    clc
    lda nsk_pool_worldx_lo, x
    adc _collision_b_width
    sta _collision_b_right_lo
    lda nsk_pool_worldx_hi, x
    adc #0
    sta _collision_b_right_hi

    clc
    lda nsk_pool_worldy_lo, x
    adc _collision_b_height
    sta _collision_b_bottom
    bcc :+
        lda #$ff
        sta _collision_b_bottom
    :

    rts
.endproc

; @brief Checks whether the current collision pair boxes overlap
;
; @param[out] nsk_pool_result non-zero if the boxes overlap
.proc _pool_collision_pair_check
    lda #0
    sta nsk_pool_result

    jsr _pool_collision_boxes_calc

    ; A.left < B.right
    ldx _collision_a_index
    lda nsk_pool_worldx_hi, x
    cmp _collision_b_right_hi
    bcc :+
    bne done

    lda nsk_pool_worldx_lo, x
    cmp _collision_b_right_lo
    bcs done
    :

    ; B.left < A.right
    ldx _collision_b_index
    lda nsk_pool_worldx_hi, x
    cmp _collision_a_right_hi
    bcc :+
    bne done

    lda nsk_pool_worldx_lo, x
    cmp _collision_a_right_lo
    bcs done
    :

    ; A.top < B.bottom
    ldx _collision_a_index
    lda nsk_pool_worldy_lo, x
    cmp _collision_b_bottom
    bcs done

    ; B.top < A.bottom
    ldx _collision_b_index
    lda nsk_pool_worldy_lo, x
    cmp _collision_a_bottom
    bcs done

    lda #1
    sta nsk_pool_result

    done:
        rts
.endproc

; @brief Dispatches both sides of the current collision pair
.proc _pool_collision_pair_dispatch
    ldx _collision_a_index
    lda _collision_b_index
    sta nsk_pool_collision_other
    jaic _table_ptr, nsk_sprites_table_collision, { nsk_pool_object, x }

    ldx _collision_b_index
    lda _collision_a_index
    sta nsk_pool_collision_other
    jaic _table_ptr, nsk_sprites_table_collision, { nsk_pool_object, x }

    rts
.endproc

; @brief Ticks all pool collisions
.proc _pool_tick_collisions
    lda #0
    sta _collision_a_index

    outer:
        ldx _collision_a_index
        cpx _pool_size
        bne :+
            jmp done
        :

        jsr _pool_collision_flags_check
        bne :+
            jmp next_a
        :

        lda _collision_a_index
        clc
        adc #1
        sta _collision_b_index

    inner:
        ldx _collision_b_index
        cpx _pool_size
        bne :+
            jmp next_a
        :

        jsr _pool_collision_flags_check
        beq next_b

        jsr _pool_collision_pair_check

        lda nsk_pool_result
        beq next_b

        jsr _pool_collision_pair_dispatch

    next_b:
        inc _collision_b_index
        jmp inner

    next_a:
        inc _collision_a_index
        jmp outer

    done:
        rts
.endproc

; @brief Adds the element to the OAM list if it's visible
;
; @param[in] X Current element index
.proc _pool_draw_element

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
        jaic _table_ptr, nsk_sprites_table_draw, { nsk_pool_object, x }

    not_visible:

    rts
.endproc

; @brief Removes objects marked as deleted
.proc _pool_sweep_deleted
    ldx #0
    cpx nsk_pool_size
    bne loop
        jmp done

    loop:
        lda nsk_pool_flags, x
        and #POOL::FLAGS::DELETED
        bne :+
            jmp next
        :

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
        rts
.endproc

; @brief Ticks all pool elements in the stable pool order
.proc _pool_tick_elements
    ldx #0
    cpx _pool_size
    beq done

    loop:
        jsr _pool_tick_element

        inx
        cpx _pool_size
        bne loop

    done:
        rts
.endproc

; @brief Moves the next draw pass start index forward
.proc _pool_draw_advance_start
    inc _pool_draw_start

    lda _pool_draw_start
    cmp _pool_size
    bcc done

    lda #0
    sta _pool_draw_start

    done:
        rts
.endproc

; @brief Draws all pool elements in a rotated order
.proc _pool_draw_elements
    lda _pool_draw_start
    cmp _pool_size
    bcc :+
        lda #0
        sta _pool_draw_start
    :

    lda _pool_draw_start
    sta _pool_draw_index

    lda _pool_size
    sta _pool_draw_count

    loop:
        ldx _pool_draw_index
        jsr _pool_draw_element

        inc _pool_draw_index

        lda _pool_draw_index
        cmp _pool_size
        bcc :+
            lda #0
            sta _pool_draw_index
        :

        dec _pool_draw_count
        bne loop

    jsr _pool_draw_advance_start
    rts
.endproc

; @brief Object pool "tick" routine
; - Calculates the physics
; - Calls objects draw routines
.export nsk_pool_tick
.proc nsk_pool_tick
    push a, x, y

    lda nsk_pool_size
    sta _pool_size

    lda _pool_size
    beq done

    jsr _pool_tick_elements
    jsr _pool_tick_collisions
    jsr _pool_draw_elements

    done:

    jsr _pool_sweep_deleted

    ; Hide the rest of the sprites (if any)
    nsk_sprites_hide

    pull a, x, y

    rts
.endproc

.endif
