; @file nsk_header_mapper_consts.inc
; @brief Include module for the mapper-related constants
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_MAPPER_CONSTS__
::__NSK_HEADER_MAPPER_CONSTS__ = 1

; @brief Maximum allowed NES 2.0 mapper identifier.
;
; Accepts mapper IDs in the NES 2.0 range 0..4095 (12 bits)
::_NSK_HEADER_MAPPER_MAXID = $0fff

; @brief Maximum allowed NES 2.0 submapper identifier.
;
; Accepts submapper IDs in the NES 2.0 range 0..15 (4 bits)
::_NSK_HEADER_SUBMAPPER_MAXID = $0f

.endif
