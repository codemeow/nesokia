; @file nsk_pool_helpers.inc
; @brief Include module for the object pool helpers
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_POOL_HELPERS__
::__NSK_POOL_HELPERS__ = 1

.linecont +

.include "nsk_pool_settings.inc"

; @brief Creates the identifier with the name `nsk_pool_ ## <name>`
;
; @param[in] name The suffix of the identifier
.define _NSK_POOL_IDENT(name) .ident(.sprintf("nsk_pool_%s", name))

; @brief Declares the import of the pool variable
;
; @param[in] name Name of the argument
; @param[in] arg  Argument value (ignored)
; @param[in] init Initialization function list flag (ignored)
.macro _NSK_POOL_IMPORT_ONE name, arg, init
    .import _NSK_POOL_IDENT name
.endmacro

; @brief Declares the export of the pool variable
;
; @param[in] name Name of the argument
; @param[in] arg  Argument value (ignored)
; @param[in] init Initialization function list flag (ignored)
.macro _NSK_POOL_EXPORT_ONE name, arg, init
    .export _NSK_POOL_IDENT name
.endmacro

; @brief Copies the `<pool><name>[Y]` into `<pool><name>[X]`
;
; @param[in] name Name of the argument
; @param[in] arg  Argument value (ignored)
; @param[in] init Initialization function list flag (ignored)
.macro _NSK_POOL_COPYY2X name, arg, init
    lda _NSK_POOL_IDENT {name}, y
    sta _NSK_POOL_IDENT {name}, x
.endmacro

; @brief Defines the pool variable of the _STACK_MAX size
;
; @param[in] name Name of the argument
; @param[in] arg  Argument value (ignored)
; @param[in] init Initialization function list flag (ignored)
.macro _NSK_POOL_DEFINE_ONE name, arg, init
    _NSK_POOL_IDENT {name}:
        .res _NSK_POOL_MAX
.endmacro

.endif