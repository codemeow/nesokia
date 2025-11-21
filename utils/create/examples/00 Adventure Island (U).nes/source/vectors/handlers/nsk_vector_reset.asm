; @file nsk_vector_reset.asm
; @brief Source module for the reset vector
;
; Part of the Nesokia project — MIT License.
.ifndef ::__NSK_VECTOR_RESET_ASM__
::__NSK_VECTOR_RESET_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "../../vectors/handlers/nsk_vector_reset.inc"
.include "../../vectors/handlers/nsk_vector_nmi.inc"
.include "../../stage/nsk_stage_list.inc"
.include "../../ppu/nsk_ppu_vars.inc"
.include "../../nsk_main.inc"

.segment "CODE"

.macro init_irq_ignore
    sei
.endmacro

.macro init_decimal_disable
    cld
.endmacro

.macro init_apuirq_disable
    ldx #( \
        NSK::CPU::APU::BITS::FRAMECOUNTER::IRQ_DISABLE | \
        NSK::CPU::APU::BITS::FRAMECOUNTER::MODE_4STEP    \
    )
    stx NSK::CPU::APU::IRQ
.endmacro

.macro init_stack_pointer
    ldx #$ff
    txs
.endmacro

.macro init_ppu_values
    lda #( \
        NSK::PPU::BITS::PPUMASK::GRAYSCALE_OFF       | \
        NSK::PPU::BITS::PPUMASK::LEFTMOST_BACK_OFF   | \
        NSK::PPU::BITS::PPUMASK::LEFTMOST_SPRITE_OFF | \
        NSK::PPU::BITS::PPUMASK::RENDER_BACK_OFF     | \
        NSK::PPU::BITS::PPUMASK::RENDER_SPRITES_OFF  | \
        NSK::PPU::BITS::PPUMASK::RED_NORMAL          | \
        NSK::PPU::BITS::PPUMASK::GREEN_NORMAL        | \
        NSK::PPU::BITS::PPUMASK::BLUE_NORMAL           \
    )
    sta ppu_temp_mask
    sta NSK::CPU::PPU::PPUMASK

    lda #( \
        NSK::PPU::BITS::PPUCTRL::NAMETABLE_2000      | \
        NSK::PPU::BITS::PPUCTRL::VRAM_INC_1          | \
        NSK::PPU::BITS::PPUCTRL::SPRITE_TABLE_1000   | \
        NSK::PPU::BITS::PPUCTRL::TILES_TABLE_0000    | \
        NSK::PPU::BITS::PPUCTRL::SPRITE_8x8          | \
        NSK::PPU::BITS::PPUCTRL::PPU_SLAVE           | \
        NSK::PPU::BITS::PPUCTRL::NMI_DISABLE           \
    )
    sta ppu_temp_control
    sta NSK::CPU::PPU::PPUCTRL
.endmacro

.macro init_apu_values
    lda #$00 ; @todo - Set every value as bitflags instead
    sta NSK::CPU::APU::DMC::IRQ
.endmacro

.macro init_vblank_wait
.scope
    bit NSK::CPU::PPU::PPUSTATUS
    wait:
        bit NSK::CPU::PPU::PPUSTATUS
        bpl wait
.endscope
.endmacro

; moves sprites to x=$ff
.macro init_oam_reset
    ldx #$00
    lda #$ff
    clear_oam:
        sta NSK::CPU::RAM::OAMBUFFER, x
        inx
        inx
        inx
        inx
        bne clear_oam
.endmacro

.macro init_zeropage_reset
    jsr nsk_nmi_sleep_init
    jsr nsk_stage_init

    lda #$00
    sta ppu_temp_scroll_x
    sta ppu_temp_scroll_y
.endmacro

; @brief Reset handler routine
; @note Never returns
.export nsk_vector_reset
.proc nsk_vector_reset
    init_irq_ignore
    init_decimal_disable
    init_apuirq_disable
    init_stack_pointer
    init_ppu_values
    init_apu_values

    init_vblank_wait

    init_oam_reset
    init_zeropage_reset

    init_vblank_wait

    jmp nsk_main
.endproc

.endif