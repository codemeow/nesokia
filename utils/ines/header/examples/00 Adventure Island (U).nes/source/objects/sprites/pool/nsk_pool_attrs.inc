; @file nsk_stack_pool.inc
; @brief Include module for the object pool attributes list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_ATTRS__
::__NSK_POOL_ATTRS__ = 1

; @brief List of the object pool attributes
.macro _NSK_POOL_ATTRS OP
    OP object           ; Object index
    OP flags            ; Object flags

    OP worldx_hi        ; ⎡ Object world X coordinates
    OP worldx_lo        ; ⎢
    OP worldx_frac      ; ⎣
    OP worldy_lo        ; ⎡ Object world Y coordinates
    OP worldy_frac      ; ⎣

    OP vectorx_lo       ; ⎡ Object vector X force
    OP vectorx_frac     ; ⎣
    OP vectory_lo       ; ⎡ Object vector Y force
    OP vectory_frac     ; ⎣

    OP timer_a          ; Object Timer A
.endmacro

; @brief Object settings
.scope POOL
    ; @brief List of flags
    .scope FLAGS
        ; Is this object screen-fixed?
        ; If so, the nsk_pool_worldy and nsk_pool_worldx are used as
        ; screen coordinates
        FIXED     = 1 << 0
        ; Is this object affected by gravity?
        GRAVITY   = 1 << 1
        ; Is this object has a collision box?
        COLLISION = 1 << 2
    .endscope

.endscope

.endif