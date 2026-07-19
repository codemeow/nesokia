; @file nsk_vector_nmi.inc
; @brief Configurable NMI handler for NES frame projects
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_VECTOR_NMI__
::__NSK_VECTOR_NMI__ = 1

.linecont +

.include "../nsk_frame_configs.inc"

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