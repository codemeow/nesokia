; @file nsk_vector_nmi.asm
; @brief Source module for the NMI handler vector
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_VECTOR_NMI_ASM__
::__NSK_VECTOR_NMI_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../../vectors/handlers/nsk_vector_nmi.inc"
.include "../../ppu/nsk_ppu_vars.inc"
.include "../../cnrom/nsk_map003_bankswitch.inc"

.segment "ZEROPAGE"

; @brief NMI detector for main
.exportzp nsk_nmi_sleep_flag
nsk_nmi_sleep_flag:
    .res 1

; @brief Current CHR ROM bank index
_nsk_bank_index:
    .res 1
; @brief Current frame
_nsk_bank_frame:
    .res 1

; @brief Period in frames to change to the next bank
_NSK_BANKSWITCH_FRAME = 30

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

; @brief Changes the CHR ROM bank periodically
.proc _bank_switch
    inc _nsk_bank_frame
    lda _nsk_bank_frame
    cmp #_NSK_BANKSWITCH_FRAME
    bne done

        lda #$00
        sta _nsk_bank_frame

        inc _nsk_bank_index
        lda _nsk_bank_index
        cmp #NSK_MAP003_BANKCOUNT
        bne :+
            lda #$00
            sta _nsk_bank_index
        :
    done:

    lda _nsk_bank_index
    jsr nsk_map003_bankswitch

    rts
.endproc

; @brief NMI handler routine
.export nsk_vector_nmi
.proc nsk_vector_nmi
    push a, x, y

    nsk_todo "nsk_vector_nmi - Array-to-PPUADDR interpreter"

    jsr _bank_switch

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

    pull a
    rts
.endproc

; @brief Inits the NMI-related variables
.export nsk_nmi_vars_init
.proc nsk_nmi_vars_init
    push a

    lda #$00
    sta _nsk_bank_frame
    sta _nsk_bank_index

    pull a
    rts
.endproc

.endif