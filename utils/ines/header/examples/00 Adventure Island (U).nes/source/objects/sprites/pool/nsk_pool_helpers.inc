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
.define _NSK_POOL_IDENT(name) .ident(.sprintf("nsk_pool_%s", .string(name)))

; @brief Declares the import of the pool variable
;
; @param[in] name The suffix of the identifier
.macro _NSK_POOL_IMPORT_ONE name
    .import _NSK_POOL_IDENT name
.endmacro

; @brief Declares the export of the pool variable
;
; @param[in] name The suffix of the identifier
.macro _NSK_POOL_EXPORT_ONE name
    .export _NSK_POOL_IDENT name
.endmacro

; @brief Copies the `<pool><name>[Y]` into `<pool><name>[X]`
;
; @param[in] name The suffix of the identifier
.macro _NSK_POOL_COPYY2X name
    lda _NSK_POOL_IDENT {name}, y
    sta _NSK_POOL_IDENT {name}, x
.endmacro

; @brief Defines the pool variable of the _STACK_MAX size
;
; @param[in] name The suffix of the identifier
.macro _NSK_POOL_DEFINE_ONE name
    _NSK_POOL_IDENT {name}:
        .res _NSK_POOL_MAX
.endmacro

.endif