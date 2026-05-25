; @file nsk_stage_init.inc
; @brief Include module for the stage init routines
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_INIT__
::__NSK_STAGE_INIT__ = 1

.include "nsk_common_meta.inc"

.ifndef ::__NSK_STAGE_INIT_ASM__
    ; @brief Inits all the stages routines
    .import nsk_stage_init
.endif

.endif