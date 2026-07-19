; @file nsk_frame_entry.asm
; @brief Source module for the main routine
;
; Part of the Nesokia project — MIT License.
.ifndef ::__NSK_FRAME_ENTRY_ASM__
::__NSK_FRAME_ENTRY_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"
.include "nsk_frame_configs.inc"

.if .defined(::NSK_FEATURE_MAIN) \
    .and ::NSK_FEATURE_MAIN = 1


.if (!.defined(::NSK_FEATURE_NMI))      \
    .or (                               \
        .defined(::NSK_FEATURE_NMI)     \
        .and ::NSK_FEATURE_NMI <> 1     \
    )

    .error "NSK_FEATURE_MAIN and NSK_FEATURE_NMI should be both set or unset"
.endif

.import NSK_MAIN_FUNCTION

.segment NSK_SEGMENT_BSS

; @brief NMI processing detector
.export nsk_nmi_flag
nsk_nmi_flag:
    .res 1

.segment NSK_MAIN_SEGMENT

; @brief Pre-main initialization
.proc _main_init
    lda #0
    sta nsk_nmi_flag

    rts
.endproc

; @brief Low-level main entry point processor
.export nsk_frame_main
.proc nsk_frame_main
    jsr _main_init

    main:
        jsr NSK_MAIN_FUNCTION

    inc nsk_nmi_flag
    
    :
        lda nsk_nmi_flag
        bne :-

    jmp main
.endproc

.endif

.endif
