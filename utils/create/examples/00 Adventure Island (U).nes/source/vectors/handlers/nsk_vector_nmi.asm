; @file nsk_vector_nmi.asm
; @brief Source module for the NMI handler vector
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_VECTOR_NMI_ASM__
::__NSK_VECTOR_NMI_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../vectors/handlers/nsk_vector_nmi.inc"
.include "../../ppu/nsk_ppu_vars.inc"

.segment "ZEROPAGE"

; @brief NMI detector for main
.exportzp nsk_nmi_sleep_flag
nsk_nmi_sleep_flag:
    .res 1

nsk_debug "TEST BANK"
test_bank:
    .res 1

frame_counter:
    .res 1


.segment "BANKREG"

; @brief Locks the BANKREG segment $ff value to resolve BUS conflicts
bankreg:
    .byte $ff

.segment "CODE"

; @brief Updates sprites data
.proc _sprites_update
    push a

    lda #$00
    sta NSK::CPU::PPU::OAMADDR
    lda #.hibyte(NSK::CPU::RAM::OAMBUFFER)
    sta NSK::CPU::PPU::OAMDMA

    pull a
    rts
.endproc

; @brief Resets the NMI flag to detect passed VBLANK in main
.proc _flag_reset
    push a

    lda #$00
    sta nsk_nmi_sleep_flag

    pull a
    rts
.endproc

; @brief Updates PPUMASK value
.proc _ppumask_update
    push a

    lda ppu_temp_mask
    sta NSK::CPU::PPU::PPUMASK

    pull a
    rts
.endproc

; @brief Updates PPUCTRL value
.proc _ppuctrl_update
    push a

    lda ppu_temp_control
    sta NSK::CPU::PPU::PPUCTRL

    pull a
    rts
.endproc

; @brief Updates PPUSCROLL value
.proc _ppuscroll_update
    push a

    bit NSK::CPU::PPU::PPUSTATUS
    lda ppu_temp_scroll_x
    sta NSK::CPU::PPU::PPUSCROLL
    lda ppu_temp_scroll_y
    sta NSK::CPU::PPU::PPUSCROLL

    pull a
    rts
.endproc

; @brief NMI handler routine
.export nsk_vector_nmi
.proc nsk_vector_nmi
    push a, x, y

    nsk_todo "nsk_vector_nmi - Array-to-PPUADDR interpreter"

    inc frame_counter
    lda frame_counter
    cmp #60
    bne same

        lda #$00
        sta frame_counter

        inc test_bank
        lda test_bank
        cmp #$04
        bne ok
            lda #$00
            sta test_bank
        ok:

    same:

    lda test_bank
    sta bankreg

    jsr _ppumask_update
    jsr _sprites_update
    jsr _ppuctrl_update
    jsr _ppuscroll_update

    jsr _flag_reset

    pull a, x, y

    rti
.endproc

; @brief Inits the NMI detector for main
.export nsk_nmi_sleep_init
.proc nsk_nmi_sleep_init
    push a

    lda #$00
    sta nsk_nmi_sleep_flag

    sta test_bank
    sta frame_counter

    pull a
    rts
.endproc

.endif