; @file nsk_header_prgrom_consts.asm
; @brief Include module for the PRG-ROM constants
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_PRGROM_CONSTS__
::__NSK_HEADER_PRGROM_CONSTS__ = 1

; @def NSK_HEADER_PRGROM_QUANTUM
; @brief Quantization unit for PRG-ROM size in simple encoding.
;
; Defines the minimum PRG-ROM allocation step in NES headers.
; One unit equals 16 KiB (`$4000` bytes).
::NSK_HEADER_PRGROM_QUANTUM = $4000

.endif

