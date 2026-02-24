; @file nsk_pool_add.inc
; @brief Include module for the object pool add routine
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_ADD__
::__NSK_POOL_ADD__ = 1

.linecont +

.include "nsk_pool_attrs.inc"
.include "nsk_pool_helpers.inc"

; @brief Adds new sprite object to the object pool
.macro nsk_pool_add \
    index,
    flags,
    worldx,
    worldy,
    vectorx,
    vectory,
    timer

    .local done

    push a,y

    lda nsk_stack_size

;-- Генератор как в спрайтах

    pull a,y

.endmacro

.endif