; @file nsk_default_mapper.inc
; @brief Default mapper configuration
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_DEFAULT_MAPPER__
::__NSK_DEFAULT_MAPPER__ = 1

; Mapper initialization is enabled unless the project disables it explicitly.
.if !.defined(::NSK_FEATURE_MAPPER)
    ::NSK_FEATURE_MAPPER = 1
.endif

.endif