; @file nsk_header_magic.asm
; @brief Include module for the NES 2.0 magic number generation
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_MAGIC__
::__NSK_HEADER_MAGIC__ = 1

; @macro nsk_header_magic
; @brief Inserts the standard NES ROM magic signature.
;
; This macro generates the first four bytes of an iNES/NES 2.0 header:
; - `"NES"` — ASCII identification string  
; - `0x1A`  — end-of-file marker
;
; Together these form the "magic number" that allows emulators
; and tools to recognize a valid NES ROM image.
.macro nsk_header_magic
    nes:
        .byte "NES"
    eof:
        .byte $1a
.endmacro

.endif

