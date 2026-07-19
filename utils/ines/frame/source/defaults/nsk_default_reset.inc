; @file nsk_default_reset.inc
; @brief Default reset configuration
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_DEFAULT_RESET__
::__NSK_DEFAULT_RESET__ = 1

; Default 2C02 is applied with black backdrop
.if !.defined(NSK_RESET_BACKDROP_SET)
    ::NSK_RESET_BACKDROP_SET = 1
.endif

.if !.defined(NSK_RESET_BACKDROP_COLOR)
    ::NSK_RESET_BACKDROP_COLOR = $0f
.endif

; ZP clearing
.if !.defined(NSK_RESET_ZPCLEAR)
    ::NSK_RESET_ZPCLEAR = 1
.endif

; Sprites clearing
.if !.defined(NSK_RESET_SPRITESCLEAR)
    ::NSK_RESET_SPRITESCLEAR = 1
.endif

.endif