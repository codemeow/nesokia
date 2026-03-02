; @file nsk_pool_add.inc
; @brief Include module for the object pool add routine
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_ADD__
::__NSK_POOL_ADD__ = 1

.linecont +

.include "nsk_pool_attrs.inc"
.include "nsk_pool_helpers.inc"
.include "nsk_pool_settings.inc"

; Settings to ease reuse outside of the project
_PA_OBJECTS_MAX     = _NSK_POOL_MAX
_PA_OBJECTS_COUNT   = nsk_pool_size

; Argument types
;
; @note The module DOES NOT support "ZP indexed indirect"
.define _PA_ARG_TYPE_NONINDEXED 0 ; @< Argument type "Immediate/Absolute/ZP"
.define _PA_ARG_TYPE_ABSINDX    1 ; @< Argument type "Absolute/ZP indexed with X"
.define _PA_ARG_TYPE_ABSINDY    2 ; @< Argument type "Absolute/ZP indexed with Y"

; @brief Starts the argc check sequence for the nsk_pool_add
.macro _NSK_POOL_ADD_ARGS_START
    _nsk_add_args_counter .set 0
.endmacro

; @brief Executes the expected count calculation for the nsk_pool_add
;
; @param[in] name Name of the argument (ignored)
; @param[in] arg  Argument value (ignored)
; @param[in] init Initialization function list flag
;
; @note The correct use of this macro is via using it as the OP macro in the
; global argument list macro, eg. `_NSK_POOL_ATTRS _NSK_POOL_ADD_ARGS_OP`
; @note The _NSK_POOL_ADD_ARGS_START macro should be called beforehand
.macro _NSK_POOL_ADD_ARGS_ENTRY name, arg, init
    .if init = 1
        _nsk_add_args_counter .set _nsk_add_args_counter + 1
    .endif
.endmacro

; @brief Number of expected args in the nsk_pool_add
.macro _NSK_POOL_ADD_ARGS_CHECK argc
    .assert \
        _nsk_add_args_counter = argc,   \
        error,                          \
        "nsk_pool_add args list mismatch"
.endmacro

; @brief Local macro variable "switch" constructor
.define _NSK_VAR_SWITCH_(index) .ident(.sprintf("switch_%d", index))

; @brief Local macro variable "switch" constructor
.define _NSK_VAR_ARGTYPE_(index) .ident(.sprintf("argtype_%d", index))

; @brief Detects the argument type
;
; @param[in]  name Name of the argument (ignored)
; @param[in]  arg  Argument to check
; @param[in]  init Initialization function list flag
;
; @param[in]  _argindex      Index of the current argument
; @param[out] ARGTYPE<N>     Argument type
; @param[out] _xclobbered    Flag to check if the X register would be clobbered
.macro _PA_ARG_CHECK name, arg, init

    .if (.xmatch (.left (1, {arg}), #))
        _NSK_VAR_ARGTYPE_ { _argindex } .set _PA_ARG_TYPE_NONINDEXED

    .elseif (.xmatch (.right (1, {arg}), x))
        _NSK_VAR_ARGTYPE_ { _argindex } .set _PA_ARG_TYPE_ABSINDX
        _xclobbered .set 1

    .elseif (.xmatch (.right (1, {arg}), y))
        _NSK_VAR_ARGTYPE_ { _argindex } .set _PA_ARG_TYPE_ABSINDY

    .else
        _NSK_VAR_ARGTYPE_ { _argindex } .set _PA_ARG_TYPE_NONINDEXED
    .endif

    _argindex .set _argindex + 1
.endmacro

; @brief Pushes the index of the selected type to stack
;
; @param argtype Argument type
.macro _PA_INDEX_PUSH  \
    argtype

    .if argtype = _PA_ARG_TYPE_ABSINDX
        txa
        pha

    .elseif argtype = _PA_ARG_TYPE_ABSINDY
        tya
        pha
    .endif
.endmacro

; @brief Writes one single byte (core routine)
;
; @param[in]        name          Name of the argument
; @param[in]        arg           Argument to check
; @param[in]        init          Initialization function list flag (ignored)
;
; @param[in]        _argindex     Index of the current argument
; @param[out]       ARGTYPE<N>    Argument type
; @param[out]       POOL<name>    Target to write
; @param[in,out]    _currenttype  Current active argument type
;
; @note Uses X register for <target> indexing,
;       Y register for argument indexing (nb. arg,x == arg,y)
.macro _PA_ARG_WRITE  name, arg, init
    .if _NSK_VAR_ARGTYPE_ { _argindex } = _PA_ARG_TYPE_NONINDEXED
        lda arg

    .else
        .if _NSK_VAR_ARGTYPE_ { _argindex } <> _currenttype
            pla
            tay
            _currenttype .set _NSK_VAR_ARGTYPE_ { _argindex }
        .endif

        lda .left (.tcount ({arg}) - 2, {arg}), y
    .endif

    sta _NSK_POOL_IDENT { name }, x

    _argindex .set _argindex + 1
.endmacro

; @brief Pushes the X register to the stack to allow the use of it
; as OAMBUFFER indexing without clobbering
;
; @note Semantics only macro
.macro _PA_X_PUSH
    txa
    pha
.endmacro

; @brief Pulls the X register from the stack
;
; @note Semantics only macro
.macro _PA_X_PULL
    pla
    tax
.endmacro

; @brief Pushes the Y to stack if it's going to be clobbered later
;
; @param[in] _xclobbered X clobbered check flag
;
; @note Note the check against X and not Y as if the user provided values
;       use Y for indexing the register could be used "as is" without
;       clobbering
.macro _PA_Y_PUSH

    .if _xclobbered = 1
        tya
        pha
    .endif
.endmacro

; @brief Pushes the Y to stack if it's going to be clobbered later
;
; @param[in] _xclobbered X clobbered check flag
;
; @note Note the check against X and not Y as if the user provided values
;       use Y for indexing the register could be used "as is" without
;       clobbering
.macro _PA_Y_PULL

    .if _xclobbered = 1
        pla
        tay
    .endif
.endmacro

; @brief Adds register switch to the build plan if needed
;
; @param[in]  index         Index of the argument
;
; @param[in]  ARGTYPE<N>    Argument type
; @param[out] SWITCH<N>     Switch value type
; @param[out] _switchcount  Number of used switches
.macro _PA_SWITCH_ADD  index
    .if (_NSK_VAR_ARGTYPE_ { index } <> _PA_ARG_TYPE_NONINDEXED) && \
        (_NSK_VAR_ARGTYPE_ { index } <> _currenttype)

        .repeat _NSK_POOL_ATTRSCOUNT, i
            .if _switchcount = i
                _NSK_VAR_SWITCH_ {i} .set _NSK_VAR_ARGTYPE_ { index }
            .endif
        .endrep

        _switchcount .set _switchcount + 1
        _currenttype .set _NSK_VAR_ARGTYPE_ { index }
    .endif
.endmacro

; @brief Builds plan to the push/pull order for the arguments
;
; Pushes the used registers in the reverse order to maintain the pull order
; match later
.macro _PA_PLAN_BUILD
    .repeat _NSK_POOL_ATTRSCOUNT, i
        _NSK_VAR_SWITCH_ { i } .set _PA_ARG_TYPE_NONINDEXED
    .endrep

    _switchcount .set 0
    _currenttype .set _PA_ARG_TYPE_ABSINDY

    .repeat _NSK_POOL_ATTRSCOUNT, i
        _PA_SWITCH_ADD i
    .endrep

    .repeat _NSK_POOL_ATTRSCOUNT - 1, i
        _checkindex .set _NSK_POOL_ATTRSCOUNT - 1 - i

        .if _checkindex > 0 && _switchcount <= _checkindex
            _PA_INDEX_PUSH _NSK_VAR_SWITCH_  { _checkindex - 1 }
        .endif
    .endrep
.endmacro

; @brief Adds new sprite object to the object pool
;
; @param[in] _NSK_POOL_ATTRS-listed arguments with the init flag = 1
.macro nsk_pool_add \
    object,         \
    flags,          \
    worldx_hi,      \
    worldx_lo,      \
    worldy_lo,      \
    vectorx_lo,     \
    vectory_lo,     \
    timer_a

    _NSK_POOL_ADD_ARGS_START
    _NSK_POOL_ATTRS _NSK_POOL_ADD_ARGS_ENTRY
    _NSK_POOL_ADD_ARGS_CHECK .paramcount

    .local done

    lda _PA_OBJECTS_COUNT
    cmp #_PA_OBJECTS_MAX
    beq done

    .repeat .paramcount, i
        _NSK_VAR_ARGTYPE_ { i } .set _PA_ARG_TYPE_NONINDEXED
    .endrep

    _xclobbered .set 0
    _argindex   .set 0

    _NSK_POOL_ATTRS \
        _PA_ARG_CHECK,  \
        { object     }, \
        { flags      }, \
        { worldx_hi  }, \
        { worldx_lo  }, \
        { #0         }, \
        { worldy_lo  }, \
        { #0         }, \
        { vectorx_lo }, \
        { #0         }, \
        { vectory_lo }, \
        { #0         }, \
        { timer_a    }

    _PA_X_PUSH
    _PA_Y_PUSH

    _PA_PLAN_BUILD

    _argindex    .set 0
    _currenttype .set _PA_ARG_TYPE_ABSINDY

    _NSK_POOL_ATTRS \
        _PA_ARG_WRITE,  \
        { object     }, \
        { flags      }, \
        { worldx_hi  }, \
        { worldx_lo  }, \
        { #0         }, \
        { worldy_lo  }, \
        { #0         }, \
        { vectorx_lo }, \
        { #0         }, \
        { vectory_lo }, \
        { #0         }, \
        { timer_a    }

    inc _PA_OBJECTS_COUNT

    _PA_Y_PULL
    _PA_X_PULL

    done:

.endmacro

.endif