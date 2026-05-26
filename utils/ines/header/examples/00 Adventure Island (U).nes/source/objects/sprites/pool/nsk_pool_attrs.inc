; @file nsk_stack_pool.inc
; @brief Include module for the object pool attributes list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_ATTRS__
::__NSK_POOL_ATTRS__ = 1

.linecont +

; @brief Starts the attributes list check
.macro _NSK_OP_LIST_START
    _nsk_op_counter .set 0
.endmacro

; @brief Declares the attributes list entry
;
; @param[in] OP    Operation to execute
; @param[in] name  Name of the attribute
; @param[in] value Value of the attribute
; @param[in] init  Is this entry should be in the init list?
.macro _NSK_OP_LIST_ENTRY OP, name, value, init
    OP name, { value }, init

    _nsk_op_counter .set _nsk_op_counter + 1
.endmacro

; @brief Checks the number of the elements in the list against the declared
; list passed to the macro
;
; @note Makes exception for a single "operation-only" declaration
.macro _NSK_OP_LIST_CHECK argc
    .if argc <> 1
        .assert                             \
            (argc - 1) = _nsk_op_counter,   \
            error,                          \
            "Attributes list args mismatch"
    .endif
.endmacro

; @brief List of the object pool attributes
;
; @param[in] OP  Operation macro to execute
; @param[in] ... Arguments values
.macro _NSK_POOL_ATTRS \
    OP,                     \
    object,                 \
    flags,                  \
    worldx_hi,              \
    worldx_lo,              \
    worldx_frac,            \
    worldy_lo,              \
    worldy_frac,            \
    vectorx_lo,             \
    vectorx_frac,           \
    vectory_lo,             \
    vectory_frac,           \
    timer_a,                \
    data_id

    ;              Operation   name             value         in add ?
    _NSK_OP_LIST_START
    _NSK_OP_LIST_ENTRY OP, "object",        { object        },   1
    _NSK_OP_LIST_ENTRY OP, "flags",         { flags         },   1

    _NSK_OP_LIST_ENTRY OP, "worldx_hi",     { worldx_hi     },   1
    _NSK_OP_LIST_ENTRY OP, "worldx_lo",     { worldx_lo     },   1
    _NSK_OP_LIST_ENTRY OP, "worldx_frac",   { worldx_frac   },   0
    _NSK_OP_LIST_ENTRY OP, "worldy_lo",     { worldy_lo     },   1
    _NSK_OP_LIST_ENTRY OP, "worldy_frac",   { worldy_frac   },   0

    _NSK_OP_LIST_ENTRY OP, "vectorx_lo",    { vectorx_lo    },   1
    _NSK_OP_LIST_ENTRY OP, "vectorx_frac",  { vectorx_frac  },   1
    _NSK_OP_LIST_ENTRY OP, "vectory_lo",    { vectory_lo    },   1
    _NSK_OP_LIST_ENTRY OP, "vectory_frac",  { vectory_frac  },   1

    _NSK_OP_LIST_ENTRY OP, "timer_a",       { timer_a       },   1
    _NSK_OP_LIST_ENTRY OP, "data_id",       { data_id       },   1
    _NSK_OP_LIST_CHECK .paramcount
.endmacro

; @brief Initialize the attribute counting code
.macro _NSK_POOL_ATTRSCOUNT_INIT
    ::_nsk_attrs_counter .set 0
.endmacro

; @brief Attribute counting iterator
;
; @param[in] name Name of the argument (ignored)
; @param[in] arg  Argument value (ignored)
; @param[in] init Initialization function list flag (ignored)
.macro _NSK_POOL_ATTRSCOUNT_OP name, arg, init
    ::_nsk_attrs_counter .set ::_nsk_attrs_counter + 1
.endmacro

; @brief Attribute counting accessor
.define _NSK_POOL_ATTRSCOUNT ::_nsk_attrs_counter

; Initialize the attribute count value
_NSK_POOL_ATTRSCOUNT_INIT
_NSK_POOL_ATTRS _NSK_POOL_ATTRSCOUNT_OP

; @brief Object settings
.scope POOL
    ; @brief Gravity settings
    .scope GRAVITY
        ; @brief Gravity acceleration low byte
        ACCEL_LO   = $00

        ; @brief Gravity acceleration fractional byte
        ACCEL_FRAC = $60

        ; @brief Maximum falling speed low byte
        MAX_LO     = $0a

        ; @brief Maximum falling speed fractional byte
        MAX_FRAC   = $00
    .endscope

    ; @brief List of flags
    .scope FLAGS
        ; Is this object screen-fixed?
        ; If so, the nsk_pool_worldy and nsk_pool_worldx are used as
        ; screen coordinates
        FIXED     = 1 << 0
        ; Is this object affected by force vectors
        VECTORS   = 1 << 1
        ; Is this object affected by gravity?
        GRAVITY   = 1 << 2
        ; Is this object has a collision box?
        COLLISION = 1 << 3
        ; Should this object be removed at the end of the pool tick?
        DELETED   = 1 << 4
    .endscope
.endscope

.endif
