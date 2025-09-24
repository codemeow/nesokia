; @file nsk_header_mapper_consts.asm
; @brief Include module for the mapper-related constants
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_MAPPER_CONSTS__
::__NSK_HEADER_MAPPER_CONSTS__ = 1

; @def _NSK_HEADER_MAPPER_MAXID
; @brief Maximum allowed NES 2.0 mapper identifier.
;
; Accepts mapper IDs in the NES 2.0 range 0..4095 (12 bits)
::_NSK_HEADER_MAPPER_MAXID = $0fff

.endif
