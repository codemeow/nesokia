; @file nsk_frame_configs.inc
; @brief Include module for configuration inclusion
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_FRAME_CONFIGS__
::__NSK_FRAME_CONFIGS__ = 1

; Configs
.include "nsk_frame_segments.inc"
.include "nsk_frame_main.inc"
.include "header/nsk_header_config.inc"
.include "vectors/nsk_frame_reset.inc"
.include "vectors/nsk_frame_nmi.inc"
.include "vectors/reset/nsk_frame_constructor.inc"
.include "mapper/nsk_frame_mapper.inc"

; Defaults override
.include "nsk_frame_defaults.inc"

.endif