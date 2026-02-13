; @file nsk_util_rand8.asm
; @brief Source module for the random routines
;
; Math source: "cc65 rand":
;     https://en.wikipedia.org/wiki/Linear_congruential_generator
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_UTIL_RAND8_ASM__
::__NSK_UTIL_RAND8_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../utils/nsk_util_rand8.inc"

.segment "RODATA"

; Random seed
_rand_seed:
    .byte $de, $ad, $b0, $0b
; Random step
_rand_step:
    .byte $27, $59, $41, $31

.segment "BSS"

; Random counter
_rand_holder:
    .res 4

.segment "CODE"

; @brief Returns "random" number 0..255
;
; @param[out] A Random number
.export nsk_util_rand8
.proc nsk_util_rand8
    ; Multiply the 32-bit seed by 0x01010101 without real multiplication.
    ; 0x01010101 = 1 + (1<<8) + (1<<16) + (1<<24).
    ; Cascaded ADCs propagate lower-byte values and carries into higher bytes,
    ; effectively mixing all bytes of the seed together.
    ; This is not an exact arithmetic multiply, but provides fast diffusion
    ; suitable for a lightweight PRNG on 6502.
    ;
    clc
    lda _rand_holder + 0
    adc _rand_holder + 1
    sta _rand_holder + 1
    adc _rand_holder + 2
    sta _rand_holder + 2
    adc _rand_holder + 3
    sta _rand_holder + 3

    ; _rand_holder += $31415927
    clc
    lda _rand_holder + 0
    adc _rand_step   + 0
    sta _rand_holder + 0
    lda _rand_holder + 1
    adc _rand_step   + 1
    sta _rand_holder + 1
    lda _rand_holder + 2
    adc _rand_step   + 2
    sta _rand_holder + 2
    lda _rand_holder + 3
    adc _rand_step   + 3
    sta _rand_holder + 3

    rts
.endproc

; Initialize the randomizer
.export nsk_util_srand8
.proc nsk_util_srand8
    push a

    lda _rand_seed   + 0
    sta _rand_holder + 0
    lda _rand_seed   + 1
    sta _rand_holder + 1
    lda _rand_seed   + 2
    sta _rand_holder + 2
    lda _rand_seed   + 3
    sta _rand_holder + 3

    pull a

    rts
.endproc

.endif