; @file nsk_common_jump.inc
; @brief Include module for the different jump commands
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_COMMON_JUMP__
::__NSK_COMMON_JUMP__ = 1

.include "nsk_common_push.inc"

; @brief Jump to Address by Index instruction
; Selects the address of the function from the `table` by the `index`,
; saves the current codeflow position into the stack and calls desired
; procedure/address.
;
; Example:
; ~~~
; .enum state
;     open    ; implicit = 0
;     action
;     close
; .endenum
;
; handlers:
;     .addr _handler_open
;     .addr _handler_action
;     .addr _handler_close
;
; ...
; lda state::action
; sta variable
; ...
; jai handlers, variable ; will, thus, jump to _handler_action
; ~~~
;
; @warning The procedure/code block in any handler should end with
; `rts` instruction:
; ~~~
; .proc _handler_open
;     ...
;     rts
; .endproc
; ~~~
; or
; ~~~
; _handler_open:
;    ...
;    rts
; ~~~
; @note
;  * index could be x or y register
;  * NULL-pointers ($0000) are allowed, nothing will happen
;
; @arg ramptr Temp RAM 2 bytes area to store the pointer
; @arg table Table/list of 2-bytes addresses
; @arg index Index of the table's element
.macro jai ramptr, table, index
.scope
    init:
        push a, x, y

    address:
        .if     (.xmatch({index}, x))
            txa

        .elseif (.xmatch({index}, y))
            tya

        .else
            lda index

        .endif

        asl

        tay
        lda table + 0, y
        sta ramptr + 0
        lda table + 1, y
        sta ramptr + 1

    check:
        lda ramptr + 1
        ora ramptr + 0
        beq done

    return:
        lda #>(done - 1)
        pha
        lda #<(done - 1)
        pha

    jump:
        jmp (ramptr)

    done:
        pull a, x, y
.endscope
.endmacro

; @brief Jump By Address instruction
; Takes the provided variable, remembers the original address and jumps
; to the address, written in the variable or constant address
; @note $0000 value is permitted
; @arg address Address to jump (#$xxxx or enum/define)
.macro jba address
.scope
    init:
        push a

    check:
        .if (.xmatch (.left (1, {address}), #))
            ; address = #$xxxx
            lda #<address
            ora #>address

        .elseif (.defined(::address))
            ; address = enum/define
            lda #<address
            ora #>address

        .else
            ; address = xx
            ; address = $xx
            ; address = xxxx
            ; address = $xxxx
            lda address + 1
            ora address + 0
        .endif

        beq done

    return:
        lda #>(done - 1)
        pha
        lda #<(done - 1)
        pha

    jump:
        jmp (address)

    done:
        pull a
.endscope
.endmacro

.endif