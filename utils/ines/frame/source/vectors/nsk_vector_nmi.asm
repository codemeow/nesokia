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

.segment NSK_SEGMENT_NMICODE

; @brief Resets NMI flag, allowing to process another NMI
.proc _nmi_flag
    lda #0
    sta nsk_nmi_flag
    rts
.endproc

; @brief NMI handler routine.
.proc nsk_vector_nmi
    push a, x, y

    jsr NSK_NMI_FUNCTION

    ; TODO:
    ; sprites update
    ; ppuctrl update
    ; ppuscroll update
    ; ppumask update
    ; music update

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