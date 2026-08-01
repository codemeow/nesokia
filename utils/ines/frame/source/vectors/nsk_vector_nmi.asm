; @file nsk_vector_nmi.asm
; @brief Configurable NMI handler for NES frame projects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_VECTOR_NMI_ASM__
::__NSK_VECTOR_NMI_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"
.include "../nsk_frame_configs.inc"
.include "../nsk_frame_entry.inc"
.include "../ppu/nsk_ppu_vars.inc"

.if .defined(::NSK_FEATURE_NMI) \ 
    .and ::NSK_FEATURE_NMI = 1

.if (!.defined(::NSK_FEATURE_MAIN))      \
    .or (                                \
        .defined(::NSK_FEATURE_MAIN)     \
        .and ::NSK_FEATURE_MAIN <> 1     \
    )

    .error "NSK_FEATURE_MAIN and NSK_FEATURE_NMI should be both set or unset"
.endif

.import NSK_NMI_FUNCTION

.if .defined(::NSK_NMI_UPDATEPPUCTRL) \
    .and ::NSK_NMI_UPDATEPPUCTRL = 1

    .if !.defined(::NSK_FEATURE_PPU)
        .error "NSK_NMI_UPDATEPPUCTRL requires NSK_FEATURE_PPU = 1"
    .else
        .if ::NSK_FEATURE_PPU <> 1
            .error "NSK_NMI_UPDATEPPUCTRL requires NSK_FEATURE_PPU = 1"
        .endif
    .endif
.endif

.if .defined(::NSK_NMI_UPDATEPPUSCROLL) \
    .and ::NSK_NMI_UPDATEPPUSCROLL = 1

    .if !.defined(::NSK_FEATURE_PPU)
        .error "NSK_NMI_UPDATEPPUSCROLL requires NSK_FEATURE_PPU = 1"
    .else
        .if ::NSK_FEATURE_PPU <> 1
            .error "NSK_NMI_UPDATEPPUSCROLL requires NSK_FEATURE_PPU = 1"
        .endif
    .endif
.endif

.if .defined(::NSK_NMI_UPDATEPPUMASK) \
    .and ::NSK_NMI_UPDATEPPUMASK = 1

    .if !.defined(::NSK_FEATURE_PPU)
        .error "NSK_NMI_UPDATEPPUMASK requires NSK_FEATURE_PPU = 1"
    .else
        .if ::NSK_FEATURE_PPU <> 1
            .error "NSK_NMI_UPDATEPPUMASK requires NSK_FEATURE_PPU = 1"
        .endif
    .endif
.endif

.segment NSK_SEGMENT_NMICODE

; @brief Resets NMI flag, allowing to process another NMI
.proc _nmi_flag
    lda #0
    sta nsk_nmi_flag
    rts
.endproc

.if .defined(::NSK_FEATURE_PPU)            \
    .and ::NSK_FEATURE_PPU = 1             \
    .and .defined(::NSK_NMI_UPDATEPPUCTRL) \
    .and ::NSK_NMI_UPDATEPPUCTRL = 1

; @brief Updates PPUCTRL value
.proc _ppuctrl_update
    push a

    lda nsk_ppu_temp_ctrl
    sta NSK::CPU::PPU::PPUCTRL

    pull a
    rts
.endproc
.endif

.if .defined(::NSK_FEATURE_PPU)            \
    .and ::NSK_FEATURE_PPU = 1             \
    .and .defined(::NSK_NMI_UPDATEPPUMASK) \
    .and ::NSK_NMI_UPDATEPPUMASK = 1

; @brief Updates PPUMASK value
.proc _ppumask_update
    push a

    lda nsk_ppu_temp_mask
    sta NSK::CPU::PPU::PPUMASK

    pull a
    rts
.endproc
.endif

.if .defined(::NSK_FEATURE_PPU)              \
    .and ::NSK_FEATURE_PPU = 1               \
    .and .defined(::NSK_NMI_UPDATEPPUSCROLL) \
    .and ::NSK_NMI_UPDATEPPUSCROLL = 1

; @brief Updates PPUSCROLL value
.proc _ppuscroll_update
    push a

    bit NSK::CPU::PPU::PPUSTATUS
    lda nsk_ppu_temp_scroll_x
    sta NSK::CPU::PPU::PPUSCROLL
    lda nsk_ppu_temp_scroll_y
    sta NSK::CPU::PPU::PPUSCROLL

    pull a
    rts
.endproc
.endif

.if .defined(::NSK_NMI_UPDATESPRITES) \
    .and ::NSK_NMI_UPDATESPRITES = 1

; @brief Updates sprites data
.proc _sprites_update
    push a

    lda #$00
    sta NSK::CPU::PPU::OAMADDR
    lda #.hibyte(::NSK_NMI_UPDATESPRITES_ADDR)
    sta NSK::CPU::PPU::OAMDMA

    pull a
    rts
.endproc
.endif

; @brief NMI handler routine.
.proc nsk_vector_nmi
    push a, x, y

    jsr NSK_NMI_FUNCTION

    .if .defined(::NSK_NMI_UPDATESPRITES) \
        .and ::NSK_NMI_UPDATESPRITES = 1

        jsr _sprites_update
    .endif

    .if .defined(::NSK_FEATURE_PPU)            \
        .and ::NSK_FEATURE_PPU = 1             \
        .and .defined(::NSK_NMI_UPDATEPPUCTRL) \
        .and ::NSK_NMI_UPDATEPPUCTRL = 1

        jsr _ppuctrl_update
    .endif

    .if .defined(::NSK_FEATURE_PPU)              \
        .and ::NSK_FEATURE_PPU = 1               \
        .and .defined(::NSK_NMI_UPDATEPPUSCROLL) \
        .and ::NSK_NMI_UPDATEPPUSCROLL = 1

        jsr _ppuscroll_update
    .endif

    .if .defined(::NSK_FEATURE_PPU)            \
        .and ::NSK_FEATURE_PPU = 1             \
        .and .defined(::NSK_NMI_UPDATEPPUMASK) \
        .and ::NSK_NMI_UPDATEPPUMASK = 1

        jsr _ppumask_update
    .endif

    ; TODO music update

    jsr _nmi_flag

    pull a, x, y

    rti
.endproc

; @brief NMI vector
;
; Reset and IRQ vectors belong to their respective independent segments.
.segment NSK_SEGMENT_VECTORNMI
.addr nsk_vector_nmi

.endif

.endif
