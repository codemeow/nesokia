; @file nsk_header_rom_issimple.inc
; @brief Include module for checking is the ROM size fits the simple encoding
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_ROM_ISSIMPLE__
::__NSK_HEADER_ROM_ISSIMPLE__ = 1

; @def NSK_HEADER_ROM_SIMPLELIMIT
; @brief Upper bound for simple (linear) ROM size encoding.
;
; Defines the maximum number of ROM units (PRG or CHR) that can
; be represented with simple encoding. If the unit count exceeds
; this value, exponent–multiplier encoding must be used.
::NSK_HEADER_ROM_SIMPLELIMIT = $0eff

; @macro nsk_header_rom_issimple
; @brief Check if a ROM size can be encoded using simple mode.
;
; Determines whether the given ROM size is divisible by the
; specified quantum and does not exceed the simple encoding limit.
;
; @param    size            ROM size in bytes (constant expression).
; @param    quantum         Quantization unit in bytes (e.g. 16 KiB for PRG).
; @param    out_issimple    Symbol set to 1 if the size is encodable
;                           with simple mode, or 0 otherwise.
.macro nsk_header_rom_issimple size, quantum, out_issimple
    .if                                                 \
        size .mod quantum = 0 &&                        \
        size / quantum <= ::NSK_HEADER_ROM_SIMPLELIMIT

        out_issimple = 1
    .else
        out_issimple = 0
    .endif
.endmacro

.endif

