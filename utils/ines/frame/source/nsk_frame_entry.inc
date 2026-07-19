; @file nsk_main.inc
; @brief Header module for the main routine
;
; Part of the Nesokia project — MIT License.
.ifndef ::__NSK_FRAME_ENTRY__
::__NSK_FRAME_ENTRY__ = 1

.linecont +

.ifndef ::__NSK_FRAME_ASM__
    .import nsk_nmi_flag

    .import nsk_frame_main
.endif

.endif