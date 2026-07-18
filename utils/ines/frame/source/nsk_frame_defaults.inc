; @file nsk_frame_defaults.inc
; @brief Default configuration values for Nesokia iNES frame projects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_FRAME_DEFAULTS__
::__NSK_FRAME_DEFAULTS__ = 1

; Mapper initialization is enabled unless the project disables it explicitly.
.if !.defined(::NSK_FEATURE_MAPPER)
    ::NSK_FEATURE_MAPPER = 1
.endif

.endif
