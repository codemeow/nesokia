; @file nsk_header_ram_consts.inc
; @brief Include module for the PRG/CHR-(NV)RAM-related constants
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_RAM_CONSTS__
::__NSK_HEADER_RAM_CONSTS__ = 1

; @def NSK_HEADER_RAMSIZE_MULTIPLIER
; @brief Constant multiplier used in NES 2.0 RAM size encoding.
;
; NES 2.0 header bytes 10 and 11 store RAM sizes in the form of a
; shift count. The actual size in bytes is computed as:
;
;     size = NSK_HEADER_RAMSIZE_MULTIPLIER << shift
;
; where "shift" is the 4-bit value taken from the header field.
; A shift count of 0 indicates no memory present.
;
; This multiplier is fixed at 64 bytes by the NES 2.0 specification,
; providing compact encoding for both small (e.g. 128 B, 256 B) and
; large (e.g. 8 KiB, 32 KiB) RAM sizes.
::NSK_HEADER_RAMSIZE_MULTIPLIER = 64

.endif
