; @file nsk_frame_meta.inc
; @brief Include file for the frame project use
;
; Part of the Nesokia project — MIT License.
.ifndef ::__NSK_FRAME_META__
::__NSK_FRAME_META__ = 1

; Configs
.include "vectors/reset/nsk_frame_constructor.inc"
.include "vectors/nsk_frame_nmi.inc"

; @brief Registers a constructor-like routine
;
; @param[in] routine Routine address. The routine must end with RTS.
.macro nsk_constructor routine
    .pushseg
    .segment NSK_SEGMENT_CONSTRUCTORS
        .addr routine
    .popseg
.endmacro

; NMI variables
.if .defined(::NSK_NMI_UPDATEPPUCTRL) \
    .and ::NSK_NMI_UPDATEPPUCTRL = 1

    .import nsk_ppu_temp_ctrl
.endif

.if .defined(::NSK_NMI_UPDATEPPUSCROLL) \
    .and ::NSK_NMI_UPDATEPPUSCROLL = 1

    .import nsk_ppu_temp_scroll_x
    .import nsk_ppu_temp_scroll_y
.endif

.if .defined(::NSK_NMI_UPDATEPPUMASK) \
    .and ::NSK_NMI_UPDATEPPUMASK = 1

    .import nsk_ppu_temp_mask
.endif

.endif