; @file nsk_vector_nmi.inc
; @brief Include module for the NMI handler vector
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_VECTOR_NMI__
::__NSK_VECTOR_NMI__ = 1

.ifndef ::__NSK_VECTOR_NMI_ASM__
; @brief NMI detector for main
.importzp nsk_nmi_sleep_flag
.endif

.ifndef ::__NSK_VECTOR_NMI_ASM__
; @brief Inits the NMI detector for main
.import nsk_nmi_sleep_init
.endif

.ifndef ::__NSK_VECTOR_NMI_ASM__
; @brief NMI handler routine
.import nsk_vector_nmi
.endif

.endif