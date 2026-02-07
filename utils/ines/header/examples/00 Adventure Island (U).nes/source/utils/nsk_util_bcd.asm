; @file nsk_util_bcd.inc
; @brief Source module for the BCD routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_UTIL_BCD_ASM__
::__NSK_UTIL_BCD_ASM__ = 1

.include "nsk_common_meta.inc"
.include "../utils/nsk_util_bcd.inc"

.segment "BSS"

; @brief Temporary buffer for source
_bin_src:
    .res 1
; @brief Result low part
_bcd_0:
    .res 1
; @brief Result high part
_bcd_1:
    .res 1

; @brief Output "hundreds"
.export nsk_bcd_outh
nsk_bcd_outh:
    .res 1

; @brief Output "tens"
.export nsk_bcd_outt
nsk_bcd_outt:
    .res 1

; @brief Output "units"
.export nsk_bcd_outu
nsk_bcd_outu:
    .res 1

.segment "CODE"

; @brief ; @brief Bin to BCD 8 bits converter
;
; Implements "Double dabble" algo: https://en.wikipedia.org/wiki/Double_dabble
;
; Always takes 530 - 787 cycles. Would take 466 - 682 cycles if everything is
; put to ZP
;
; @param   a                Input register
; @param   nsk_bcd_outh     Output "hundreds" value
; @param   nsk_bcd_outt     Output "tens"     value
; @param   nsk_bcd_outu     Output "units"    value
.export nsk_bin8_todec
.proc nsk_bin8_todec
    ; Input
    sta _bin_src

    push a, x

    lda #$00
    sta _bcd_0
    sta _bcd_1

    ; Number of bits
    ldx #8

    loop:

        ; Units:
        ;   (bcd0 low nibble) >= 5 ? bcd0 += 3
        lda _bcd_0
        and #$0f
        cmp #$05
        bcc u_ok
        lda _bcd_0
        clc
        adc #$03
        sta _bcd_0

        u_ok:

        ; Tens:
        ;   (bcd0 high nibble) >= 5 ? bcd0 += 3<<4 (= $30)
        lda _bcd_0
        and #$f0
        cmp #$50
        bcc t_ok
        lda _bcd_0
        clc
        adc #$30
        sta _bcd_0

        t_ok:

        ; Thousands:
        ;   (bcd1 low nibble) >= 5 ? bcd1 += 3
        lda _bcd_1
        and #$0f
        cmp #$05
        bcc h_ok
        lda _bcd_1
        clc
        adc #$03
        sta _bcd_1

        h_ok:

        ; Shift everything by 1 bit
        asl _bin_src
        rol _bcd_0
        rol _bcd_1

        dex
        bne loop

    ; Unpacking
    lda _bcd_1
    and #$0f
    sta nsk_bcd_outh

    lda _bcd_0
    lsr a
    lsr a
    lsr a
    lsr a
    sta nsk_bcd_outt

    lda _bcd_0
    and #$0f
    sta nsk_bcd_outu

    pull a, x

    rts
.endproc

.endif