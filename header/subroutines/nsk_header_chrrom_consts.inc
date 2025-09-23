; @file nsk_header_chrrom_consts.asm
; @brief Include module for the CHR-ROM constants
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_CHRROM_CONSTS__
::__NSK_HEADER_CHRROM_CONSTS__ = 1

; @def NSK_HEADER_CHRROM_QUANTUM
; @brief Quantization unit for CHR-ROM size in simple encoding.
;
; Defines the minimum CHR-ROM allocation step in NES headers.
; One unit equals 8 KiB (`$2000` bytes).
::NSK_HEADER_CHRROM_QUANTUM = $2000

.endif

