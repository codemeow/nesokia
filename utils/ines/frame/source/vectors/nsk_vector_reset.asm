; @file nsk_vector_reset.asm
; @brief Configurable reset handler for NES frame projects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_VECTOR_RESET_ASM__
::__NSK_VECTOR_RESET_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"
.include "../nsk_frame_configs.inc"

.include "../mapper/nsk_mapper_init.inc"
.include "../nsk_frame_entry.inc"

.if .defined(::NSK_FEATURE_RESET) \ 
    .and ::NSK_FEATURE_RESET = 1

.if .defined(::NSK_RESET_PPUCTRL) \
    .and ::NSK_RESET_PPUCTRL = 1

    .assert (::NSK_RESET_PPUCTRL & NSK::CPU::PPU::BITS::PPUCTRL::NMI_BIT) = 0, \
        error, "NSK_RESET_PPUCTRL must keep NMI disabled during reset"
.endif

.if .defined(::NSK_RESET_PPUMASK) \
    .and ::NSK_RESET_PPUMASK = 1

    .assert (::NSK_RESET_PPUMASK & NSK::CPU::PPU::BITS::PPUMASK::RENDER_BACK_BIT) = 0, \
        error, "NSK_RESET_PPUMASK must keep background rendering disabled during reset"
    .assert (::NSK_RESET_PPUMASK & NSK::CPU::PPU::BITS::PPUMASK::RENDER_SPRITES_BIT) = 0, \
        error, "NSK_RESET_PPUMASK must keep sprite rendering disabled during reset"
.endif

.if .defined(::NSK_FEATURE_CONSTRUCTORS) \
    .and ::NSK_FEATURE_CONSTRUCTORS = 1

    .import nsk_constructors_run
.endif

.import NSK_RESET_MAINADDR

.segment NSK_SEGMENT_RESETCODE

; @brief Waits for the next VBlank start.
.proc _nsk_reset_vblank_wait
    bit NSK::CPU::PPU::PPUSTATUS
    loop:
        bit NSK::CPU::PPU::PPUSTATUS
        bpl loop
    rts
.endproc

; @brief Clears the complete CPU zero page.
.if .defined(::NSK_RESET_ZPCLEAR) \ 
    .and ::NSK_RESET_ZPCLEAR = 1

.proc _nsk_reset_zp_clear
    lda #0
    ldx #0
    loop:
        sta $00, x
        inx
        bne loop
    rts
.endproc
.endif

; @brief Moves every buffered OAM sprite off-screen.
.if .defined(::NSK_RESET_SPRITESCLEAR) \
    .and ::NSK_RESET_SPRITESCLEAR = 1

.proc _nsk_reset_sprites_clear
    lda #$ff
    ldx #0
    loop:
        sta NSK::CPU::RAM::OAMBUFFER, x
        inx
        inx
        inx
        inx
        bne loop
    rts
.endproc
.endif

; @brief Applies initial PPU and APU register values from reset config.
.proc _nsk_reset_hardware_init
.if .defined(::NSK_RESET_PPUMASK)

    lda #NSK_RESET_PPUMASK
    sta NSK::CPU::PPU::PPUMASK
.endif

.if .defined(::NSK_RESET_PPUCTRL)

    lda #NSK_RESET_PPUCTRL
    sta NSK::CPU::PPU::PPUCTRL
.endif

.if .defined(::NSK_RESET_APUFRAME) \
    .and ::NSK_RESET_APUFRAME = 1

    lda #NSK_RESET_APUFRAME
    sta NSK::CPU::APU::IRQ
.endif

.if .defined(::NSK_RESET_APUDMC) \
    .and ::NSK_RESET_APUDMC = 1

    lda #NSK_RESET_APUDMC
    sta NSK::CPU::APU::DMC::IRQ
.endif

    rts
.endproc

.if .defined(::NSK_RESET_BACKDROP_SET) \
    .and ::NSK_RESET_BACKDROP_SET = 1     \
    .and .defined(::NSK_RESET_BACKDROP_COLOR)

; @brief Writes an optional universal background color while rendering is off.
.proc _nsk_reset_backdrop_set
    bit NSK::CPU::PPU::PPUSTATUS
    lda #>NSK::PPU::PALETTE::TILES
    sta NSK::CPU::PPU::PPUADDR
    lda #<NSK::PPU::PALETTE::TILES
    sta NSK::CPU::PPU::PPUADDR
    lda #NSK_RESET_BACKDROP_COLOR
    sta NSK::CPU::PPU::PPUDATA

    ; Returns the V-register back to ..TILES (backdrop color)
    bit NSK::CPU::PPU::PPUSTATUS
    lda #>NSK::PPU::PALETTE::TILES
    sta NSK::CPU::PPU::PPUADDR
    lda #<NSK::PPU::PALETTE::TILES
    sta NSK::CPU::PPU::PPUADDR

    rts
.endproc
.endif

; @brief Reset handler routine.
; @note Never returns: control is transferred to NSK_RESET_MAINADDR.
.proc nsk_vector_reset
    sei
    cld
    ldx #$ff
    txs

    jsr _nsk_reset_hardware_init
    jsr _nsk_reset_vblank_wait

.if .defined(::NSK_RESET_ZPCLEAR) \
    .and ::NSK_RESET_ZPCLEAR = 1

    jsr _nsk_reset_zp_clear
.endif

.if .defined(::NSK_RESET_SPRITESCLEAR) \
    .and ::NSK_RESET_SPRITESCLEAR = 1

    jsr _nsk_reset_sprites_clear
.endif

.if .defined(::NSK_FEATURE_MAPPER) \
    .and ::NSK_FEATURE_MAPPER = 1

    jsr nsk_mapper_init
.endif

    jsr _nsk_reset_vblank_wait

.if .defined(::NSK_RESET_BACKDROP_SET) \
    .and ::NSK_RESET_BACKDROP_SET = 1     \
    .and .defined(::NSK_RESET_BACKDROP_COLOR)
    
    jsr _nsk_reset_backdrop_set
.endif

.if .defined(::NSK_FEATURE_CONSTRUCTORS) \
    .and ::NSK_FEATURE_CONSTRUCTORS = 1

    jsr nsk_constructors_run
.endif

.if .defined(::NSK_FEATURE_MAIN) \
    .and ::NSK_FEATURE_MAIN = 1

    jmp nsk_frame_main

.else

    jmp NSK_MAIN_TRAMPOLINE
.endif

.endproc

; @brief Reset vector
;
; NMI and IRQ vectors belong to their respective independent segments.
.segment NSK_SEGMENT_VECTORRESET
.addr nsk_vector_reset

.endif

.endif
