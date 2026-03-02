; @file nsk_draw_helper.inc
; @brief Include module for the sprites draw routine helpers
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_DRAW_HELPERS__
::__NSK_DRAW_HELPERS__ = 1

.linecont +
.listbytes unlimited

.include "nsk_draw_vars.inc"

; Settings to ease reuse outside of the project
_SD_OAM_BUFFER    = NSK::CPU::RAM::OAMBUFFER    ; @< OAMBUFFER address
_SD_SPRITES_COUNT = nsk_sprites_used            ; @< Occupied slots counter
_SD_SPRITES_MAX   = NSK::PPU::SPRITES::MAXCOUNT ; @< Sprites slots limit

; Argument types
;
; @note The module DOES NOT support "ZP indexed indirect"
.define _SD_ARG_TYPE_NONINDEXED 0 ; @< Argument type "Immediate/Absolute/ZP"
.define _SD_ARG_TYPE_ABSINDX    1 ; @< Argument type "Absolute/ZP indexed with X"
.define _SD_ARG_TYPE_ABSINDY    2 ; @< Argument type "Absolute/ZP indexed with Y"

; Arguments list
.define _SD_ARG_POS_INDEX     0 ; @< Sprite index
.define _SD_ARG_POS_ATTRIBUTE 1 ; @< Sprite attribute (but palette) [XXXX XX00]
.define _SD_ARG_POS_PALETTE   2 ; @< Sprite palette                 [0000 00XX]
.define _SD_ARG_POS_POSX      3 ; @< Sprite X position
.define _SD_ARG_POS_POSY      4 ; @< Sprite Y position


; @brief Detects the argument type
;
; @param[in]  arg         Argument to check
; @param[out] out_argtype Argument type
.macro _SD_ARG_CHECK   \
    arg,               \
    out_argtype

    .if (.xmatch (.left (1, {arg}), #))
        out_argtype .set _SD_ARG_TYPE_NONINDEXED

    .elseif (.xmatch (.right (1, {arg}), x))
        out_argtype .set _SD_ARG_TYPE_ABSINDX

    .elseif (.xmatch (.right (1, {arg}), y))
        out_argtype .set _SD_ARG_TYPE_ABSINDY

    .else
        out_argtype .set _SD_ARG_TYPE_NONINDEXED
    .endif
.endmacro

; @brief Pushes the index of the selected type to stack
;
; @param argtype Argument type
.macro _SD_INDEX_PUSH  \
    argtype

    .if argtype = _SD_ARG_TYPE_ABSINDX
        txa
        pha

    .elseif argtype = _SD_ARG_TYPE_ABSINDY
        tya
        pha
    .endif
.endmacro

; @brief Writes one single byte (core routine)
;
; @param[in] argtype        Argument type (see _ARG_TYPE_..)
; @param[in] arg            Source argument
; @param[in] argpos         Argument position (see _ARG_POS_..)
; @param[in] currenttype    Current selected argument type
;
; @note Uses X register for OAMBUFFER indexing,
;       Y register for argument indexing (nb. arg,x == arg,y)
.macro _SD_ARG_WRITE   \
    argtype,           \
    arg,               \
    argpos,            \
    currenttype

    .if argtype = _SD_ARG_TYPE_NONINDEXED

        .if argpos <> _SD_ARG_POS_PALETTE
            lda arg

        .else
            ; Currently A contains attribute byte part
            ora arg
        .endif

    .else

        .if argtype <> currenttype
            .if argpos = _SD_ARG_POS_PALETTE
                ; See note below
                sta _SD_OAM_BUFFER, x
            .endif

            pla
            tay
            currenttype .set argtype

            .if argpos = _SD_ARG_POS_PALETTE
                ; We have no choice but to reload attribute byte part
                ; as it was destroyed by the previous `pla`
                ;
                ; Luckily this will only happen when attribute and palette
                ; passed to the `nsk_sprite_draw` have different indexes:
                ; - `{ ATTRDATA,x } + { PALDATA,y }`; or
                ; - `{ ATTRDATA,y } + { PALDATA,x }`
                ;
                ; Any other combinations will not trigger this code
                lda _SD_OAM_BUFFER, x
            .endif
        .endif

        .if argpos = _SD_ARG_POS_PALETTE
            ; As the input argument is "SOMETHING::SOMETHING, X"
            ; the "X" and "," are two different tokens.
            ; Thus -2 cuts them both
            ora .left (.tcount ({arg}) - 2, {arg}), y

        .else
            lda .left (.tcount ({arg}) - 2, {arg}), y
        .endif

    .endif

    .if argpos = _SD_ARG_POS_ATTRIBUTE
        ; This step does not require any actions

    .else
        sta _SD_OAM_BUFFER, x
        inx
    .endif
.endmacro

; @brief Pushes the X register to the stack to allow the use of it
; as OAMBUFFER indexing without clobbering
;
; @note Semantics only macro
.macro _SD_X_PUSH
    txa
    pha
.endmacro

; @brief Pulls the X register from the stack
;
; @note Semantics only macro
.macro _SD_X_PULL
    pla
    tax
.endmacro

; @brief Pushes the Y to stack if it's going to be clobbered later
;
; @param[in] argtype0  Argument type for posy
; @param[in] argtype1  Argument type for index
; @param[in] argtype2a Argument type for attributes byte part
; @param[in] argtype2b Argument type for palette byte part
; @param[in] argtype3  Argument type for posx
;
; @note Note the check against X and not Y as if the user provided values
;       use Y for indexing the register could be used "as is" without
;       clobbering
.macro _SD_Y_PUSH  \
    argtype0,      \
    argtype1,      \
    argtype2a,     \
    argtype2b,     \
    argtype3

    .if \
        argtype0  = _SD_ARG_TYPE_ABSINDX || \
        argtype1  = _SD_ARG_TYPE_ABSINDX || \
        argtype2a = _SD_ARG_TYPE_ABSINDX || \
        argtype2b = _SD_ARG_TYPE_ABSINDX || \
        argtype3  = _SD_ARG_TYPE_ABSINDX

        tya
        pha
    .endif
.endmacro

; @brief Pushes the Y to stack if it's going to be clobbered later
;
; @param[in] argtype0  Argument type for posy
; @param[in] argtype1  Argument type for index
; @param[in] argtype2a Argument type for attributes byte part
; @param[in] argtype2b Argument type for palette byte part
; @param[in] argtype3  Argument type for posx
;
; @note Note the check against X and not Y as if the user provided values
;       use Y for indexing the register could be used "as is" without
;       clobbering
.macro _SD_Y_PULL  \
    argtype0,      \
    argtype1,      \
    argtype2a,     \
    argtype2b,     \
    argtype3

    .if \
        argtype0  = _SD_ARG_TYPE_ABSINDX || \
        argtype1  = _SD_ARG_TYPE_ABSINDX || \
        argtype2a = _SD_ARG_TYPE_ABSINDX || \
        argtype2b = _SD_ARG_TYPE_ABSINDX || \
        argtype3  = _SD_ARG_TYPE_ABSINDX

        pla
        tay
    .endif
.endmacro

; @brief Adds register switch to the build plan if needed
;
; @param[in]  argtype     Argument type
; @param[out] out_switch0 Type of the first register switch
; @param[out] out_switch1 Type of the first register switch
; @param[out] out_switch2 Type of the first register switch
; @param[out] out_switch3 Type of the first register switch
; @param[out] out_count   Current number of used switches
; @param[out] out_current Current type of active switch
.macro _SD_SWITCH_ADD  \
    argtype,           \
    out_switch0,       \
    out_switch1,       \
    out_switch2,       \
    out_switch3,       \
    out_count,         \
    out_current

    .if \
        (argtype <> _SD_ARG_TYPE_NONINDEXED) && \
        (argtype <> out_current)

        .if out_count = 0
            out_switch0 .set argtype

        .elseif out_count = 1
            out_switch1 .set argtype

        .elseif out_count = 2
            out_switch2 .set argtype

        .elseif out_count = 3
            out_switch3 .set argtype
        .endif

        out_count   .set out_count + 1
        out_current .set argtype
    .endif
.endmacro

; @brief Builds plan to the push/pull order for the arguments
;
; Pushes the used registers in the reverse order to maintain the pull order
; match later
;
; @param[in] argtype0  Argument type for posy
; @param[in] argtype1  Argument type for index
; @param[in] argtype2a Argument type for attributes byte part
; @param[in] argtype2b Argument type for palette byte part
; @param[in] argtype3  Argument type for posx
.macro _SD_PLAN_BUILD  \
    argtype0,          \
    argtype1,          \
    argtype2a,         \
    argtype2b,         \
    argtype3

    switch0 .set _SD_ARG_TYPE_NONINDEXED
    switch1 .set _SD_ARG_TYPE_NONINDEXED
    switch2 .set _SD_ARG_TYPE_NONINDEXED
    switch3 .set _SD_ARG_TYPE_NONINDEXED
    switchcount .set 0

    ; As user may provide the Y-indexed
    ; arguments let us hope that it's all
    ; that is needed later
    currenttype .set _SD_ARG_TYPE_ABSINDY

    _SD_SWITCH_ADD argtype0,  \
                switch0, switch1, switch2, switch3, switchcount, currenttype
    _SD_SWITCH_ADD argtype1,  \
                switch0, switch1, switch2, switch3, switchcount, currenttype
    _SD_SWITCH_ADD argtype2a, \
                switch0, switch1, switch2, switch3, switchcount, currenttype
    _SD_SWITCH_ADD argtype2b, \
                switch0, switch1, switch2, switch3, switchcount, currenttype
    _SD_SWITCH_ADD argtype3,  \
                switch0, switch1, switch2, switch3, switchcount, currenttype

    .if switchcount = 4
        _SD_INDEX_PUSH switch3
        _SD_INDEX_PUSH switch2
        _SD_INDEX_PUSH switch1
        _SD_INDEX_PUSH switch0

    .elseif switchcount = 3
        _SD_INDEX_PUSH switch2
        _SD_INDEX_PUSH switch1
        _SD_INDEX_PUSH switch0

    .elseif switchcount = 2
        _SD_INDEX_PUSH switch1
        _SD_INDEX_PUSH switch0

    .elseif switchcount = 1
        _SD_INDEX_PUSH switch0
    .endif
.endmacro

; @brief Executes the plan - writes the values into OAMBUFFER
;
; @param[in] arg0      PosY argument
; @param[in] argtype0  Argument type for posy
; @param[in] arg1      Index argument
; @param[in] argtype1  Argument type for index
; @param[in] arg2a     Attributes byte part argument
; @param[in] argtype2a Argument type for attributes byte part
; @param[in] arg2b     Palette byte part argument
; @param[in] argtype2b Argument type for palette byte part
; @param[in] arg3      PosX argument
; @param[in] argtype3  Argument type for posx
.macro _SD_PLAN_WRITE  \
    arg0,              \
    argtype0,          \
    arg1,              \
    argtype1,          \
    arg2a,             \
    argtype2a,         \
    arg2b,             \
    argtype2b,         \
    arg3,              \
    argtype3

    currenttype .set _SD_ARG_TYPE_ABSINDY

    _SD_ARG_WRITE argtype0,  { arg0  }, _SD_ARG_POS_POSY,      currenttype
    _SD_ARG_WRITE argtype1,  { arg1  }, _SD_ARG_POS_INDEX,     currenttype
    _SD_ARG_WRITE argtype2a, { arg2a }, _SD_ARG_POS_ATTRIBUTE, currenttype
    _SD_ARG_WRITE argtype2b, { arg2b }, _SD_ARG_POS_PALETTE,   currenttype
    _SD_ARG_WRITE argtype3,  { arg3  }, _SD_ARG_POS_POSX,      currenttype

.endmacro

; @brief Calculates the byte index for the new sprite
;
; @param[out] X Index for the OAMBUFFER
.macro _SD_OAMADDR_CALC
    lda _SD_SPRITES_COUNT
    asl
    asl
    tax
.endmacro

.endif