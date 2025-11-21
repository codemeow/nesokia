; @file nsk_common_push.inc
; @brief Include module for the stack manipulations
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_COMMON_PUSH__
::__NSK_COMMON_PUSH__ = 1

; @brief Pushes arguments to stack
; @arg arg1 A, X or Y
; @arg arg2 A, X or Y (optional)
; @arg arg3 A, X or Y (optional)
;
; ~~~
; push a       equals to pha
; push x       equals to txa + pha
; push y       equals to tya + pha
; push a, x    equals to pha + txa + pha
; push a, x, y equals to pha + txa + pha + tya + pha
; ~~~
;
; @note The order of the arguments should match the corresponding psh
; Ex.:
; ~~~
; push a, x
; ...
; pull a, x
; ~~~
.macro push arg1, arg2, arg3
.scope
    check_1st:
        .ifblank arg1
            .error "PUSH instruction: first argument cannot be blank"
        .endif

        .ifnblank arg1
            .if (.match ({arg1}, a))
                pha
            .elseif (.match ({arg1}, x))
                txa
                pha
            .elseif (.match ({arg1}, y))
                tya
                pha
            .else
                .error "PUSH instruction: first argument should be a, x or y"
            .endif
        .endif

    check_2nd:
        .ifnblank arg2
            .if (.match ({arg2}, a))
                pha
            .elseif (.match ({arg2}, x))
                txa
                pha
            .elseif (.match ({arg2}, y))
                tya
                pha
            .else
                .error "PUSH instruction: second argument should be a, x or y"
            .endif
        .endif

    check_3rd:
        .ifnblank arg3
            .if (.match ({arg3}, a))
                pha
            .elseif (.match ({arg3}, x))
                txa
                pha
            .elseif (.match ({arg3}, y))
                tya
                pha
            .else
                .error "PUSH instruction: third argument should be a, x or y"
            .endif
        .endif
.endscope
.endmacro

; @brief Pull arguments from stack
; @arg arg1 A, X or Y
; @arg arg2 A, X or Y (optional)
; @arg arg3 A, X or Y (optional)
;
; ~~~
; pull a       equals to pla
; pull x       equals to pla + tax
; pull y       equals to pla + tay
; pull a, x    equals to pla + tax + pla
; pull a, x, y equals to pla + tay + pla + tax + pla
; ~~~
;
; @note The order of the arguments should match the corresponding psh
; Ex.:
; ~~~
; push a, x
; ...
; pull a, x
; ~~~
.macro pull arg1, arg2, arg3
.scope
    check_3rd:
        .ifnblank arg3
            .if (.match ({arg3}, a))
                pla
            .elseif (.match ({arg3}, x))
                pla
                tax
            .elseif (.match ({arg3}, y))
                pla
                tay
            .else
                .error "PULL instruction: third argument should be a, x or y"
            .endif
        .endif

    check_2nd:
        .ifnblank arg2
            .if (.match ({arg2}, a))
                pla
            .elseif (.match ({arg2}, x))
                pla
                tax
            .elseif (.match ({arg2}, y))
                pla
                tay
            .else
                .error "PULL instruction: second argument should be a, x or y"
            .endif
        .endif

    check_1st:
        .ifblank arg1
            .error "PULL instruction: first argument cannot be blank"
        .endif

        .ifnblank arg1
            .if (.match ({arg1}, a))
                pla
            .elseif (.match ({arg1}, x))
                pla
                tax
            .elseif (.match ({arg1}, y))
                pla
                tay
            .else
                .error "PULL instruction: first argument should be a, x or y"
            .endif
        .endif
.endscope
.endmacro

.endif