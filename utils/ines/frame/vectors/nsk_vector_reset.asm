; @file nsk_vector_reset.asm
; @brief Configurable reset handler for NES frame projects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_FRAME_VECTOR_RESET_ASM__
::__NSK_FRAME_VECTOR_RESET_ASM__ = 1

.linecont +

.include "nsk_common_consts.inc"
.include "nsk_header_reset.inc"

.if .defined(NSK_FEATURE_RESET)

.if .defined(NSK_RESET_PPUCTRL)
    .assert (NSK_RESET_PPUCTRL & NSK::CPU::PPU::BITS::PPUCTRL::NMI_BIT) = 0, \
        error, "NSK_RESET_PPUCTRL must keep NMI disabled during reset"
.endif

.if .defined(NSK_RESET_PPUMASK)
    .assert (NSK_RESET_PPUMASK & NSK::CPU::PPU::BITS::PPUMASK::RENDER_BACK_BIT) = 0, \
        error, "NSK_RESET_PPUMASK must keep background rendering disabled during reset"
    .assert (NSK_RESET_PPUMASK & NSK::CPU::PPU::BITS::PPUMASK::RENDER_SPRITES_BIT) = 0, \
        error, "NSK_RESET_PPUMASK must keep sprite rendering disabled during reset"
.endif

.if .defined(NSK_RESET_BSSCLEAR)
    .import __BSS_RUN__
    .import __BSS_SIZE__
.endif

.if .defined(NSK_HEADER_MAPPER_ID)
    .import nsk_mapper_init
.endif

.if .defined(NSK_FEATURE_CONSTRUCTORS)
    .import nsk_constructors_run
.endif

.import NSK_RESET_INITADDR
.import NSK_RESET_MAINADDR

; Reset scratch bytes are used only before optional ZP clearing. Project linker
; configs reserve $00-$0F outside of the ZEROPAGE segment for frame temporaries.
_NSK_RESET_BSS_PTR_LO = $00
_NSK_RESET_BSS_PTR_HI = $01
_NSK_RESET_BSS_LEFT_LO = $02
_NSK_RESET_BSS_LEFT_HI = $03

.segment NSK_RESET_SEGMENT

; @brief Waits for the next VBlank start.
.proc _nsk_reset_vblank_wait
    bit NSK::CPU::PPU::PPUSTATUS
loop:
    bit NSK::CPU::PPU::PPUSTATUS
    bpl loop
    rts
.endproc

; @brief Clears the complete BSS segment.
;
; Uses the linker symbols emitted by `BSS: ... define = yes`.
.if .defined(NSK_RESET_BSSCLEAR)
.proc _nsk_reset_bss_clear
    lda #<__BSS_RUN__
    sta _NSK_RESET_BSS_PTR_LO
    lda #>__BSS_RUN__
    sta _NSK_RESET_BSS_PTR_HI
    lda #<__BSS_SIZE__
    sta _NSK_RESET_BSS_LEFT_LO
    lda #>__BSS_SIZE__
    sta _NSK_RESET_BSS_LEFT_HI

loop:
    lda _NSK_RESET_BSS_LEFT_LO
    ora _NSK_RESET_BSS_LEFT_HI
    beq done

    ldy #0
    lda #0
    sta (_NSK_RESET_BSS_PTR_LO), y

    inc _NSK_RESET_BSS_PTR_LO
    bne :+
    inc _NSK_RESET_BSS_PTR_HI
:
    sec
    lda _NSK_RESET_BSS_LEFT_LO
    sbc #1
    sta _NSK_RESET_BSS_LEFT_LO
    lda _NSK_RESET_BSS_LEFT_HI
    sbc #0
    sta _NSK_RESET_BSS_LEFT_HI
    jmp loop

done:
    rts
.endproc
.endif

; @brief Clears the complete CPU zero page.
.if .defined(NSK_RESET_ZPCLEAR)
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
.if .defined(NSK_RESET_SPRITESCLEAR)
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
.if .defined(NSK_RESET_PPUMASK)
    lda #NSK_RESET_PPUMASK
    sta NSK::CPU::PPU::PPUMASK
.endif

.if .defined(NSK_RESET_PPUCTRL)
    lda #NSK_RESET_PPUCTRL
    sta NSK::CPU::PPU::PPUCTRL
.endif

.if .defined(NSK_RESET_APUFRAME)
    lda #NSK_RESET_APUFRAME
    sta NSK::CPU::APU::IRQ
.endif

.if .defined(NSK_RESET_APUDMC)
    lda #NSK_RESET_APUDMC
    sta NSK::CPU::APU::DMC::IRQ
.endif

    rts
.endproc

; @brief Writes an optional universal background color while rendering is off.
.if .defined(NSK_RESET_BACKDROP_SET) && .defined(NSK_RESET_BACKDROP_COLOR)
.proc _nsk_reset_backdrop_set
    bit NSK::CPU::PPU::PPUSTATUS
    lda #$3f
    sta NSK::CPU::PPU::PPUADDR
    lda #$00
    sta NSK::CPU::PPU::PPUADDR
    lda #NSK_RESET_BACKDROP_COLOR
    sta NSK::CPU::PPU::PPUDATA
    rts
.endproc
.endif

; @brief Reset handler routine.
; @note Never returns: control is transferred to NSK_RESET_MAINADDR.
.export nsk_vector_reset
.proc nsk_vector_reset
    sei
    cld
    ldx #$ff
    txs

    jsr _nsk_reset_hardware_init

    ; Two VBlank waits are a reset invariant, not a project configuration.
    jsr _nsk_reset_vblank_wait

.if .defined(NSK_RESET_BSSCLEAR)
    jsr _nsk_reset_bss_clear
.endif
.if .defined(NSK_RESET_ZPCLEAR)
    jsr _nsk_reset_zp_clear
.endif
.if .defined(NSK_RESET_SPRITESCLEAR)
    jsr _nsk_reset_sprites_clear
.endif

.if .defined(NSK_HEADER_MAPPER_ID)
    jsr nsk_mapper_init
.endif

    jsr _nsk_reset_vblank_wait

.if .defined(NSK_RESET_BACKDROP_SET) && .defined(NSK_RESET_BACKDROP_COLOR)
    jsr _nsk_reset_backdrop_set
.endif
.if .defined(NSK_FEATURE_CONSTRUCTORS)
    jsr nsk_constructors_run
.endif
    jsr NSK_RESET_INITADDR

    jmp NSK_RESET_MAINADDR
.endproc

.endif

.endif
