; @file nsk_pool_remove.inc
; @brief Include module for the object pool remove routine
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_REMOVE__
::__NSK_POOL_REMOVE__ = 1

.include "nsk_pool_attrs.inc"
.include "nsk_pool_helpers.inc"
.include "nsk_pool_vars.inc"
.include "../tables/nsk_sprites_tables.inc"

; @brief Remove the index-th element from the pool
;
; @param[in] index Index of the element to remove
.macro nsk_pool_remove index
    .local reduce
    .local done

    push a,x,y

    ; Sanity checks
    lda nsk_pool_size
    bne :+
        jmp done
    :
    cmp index
    bne :+
        jmp done
    :
    bcs :+
        jmp done
    :

    ; # Clean up object-specific data before moving pool slots
    ldx index
    ldy nsk_pool_object, x
    jai _table_ptr, nsk_sprites_table_clean, y

    ; # Check if the slot is not the last object
    lda nsk_pool_size
    tay
    dey
    cpy index
    bne :+
        jmp reduce
    :

    ; # Move the last element into removed slot
    ; Empty slot (target)
    ldx index

    _NSK_POOL_ATTRS _NSK_POOL_COPYY2X

    ; # Reduce the object list
    reduce:
        dec nsk_pool_size

    done:
        pull a,x,y

.endmacro

.endif
