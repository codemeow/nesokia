; @file nsk_pool_init.asm
; @brief Source module for the object pool init routine
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_INIT_ASM__
::__NSK_POOL_INIT_ASM__ = 1

.include "nsk_common_meta.inc"

.include "nsk_pool_vars.inc"

nsk_constructor _init

.segment "CODE"

.proc _init
    push a

    lda #0
    sta nsk_pool_size

    pull a

    rts
.endproc

.endif
