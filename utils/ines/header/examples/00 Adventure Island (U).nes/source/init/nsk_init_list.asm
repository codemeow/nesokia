; @file nsk_init_list.inc
; @brief Include module for the init functions list
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_INIT_LIST_ASM__
::__NSK_INIT_LIST_ASM__ = 1

.include "nsk_common_meta.inc"

.include "../init/nsk_init_list.inc"

.include "../init/list/nsk_init_ppu_disable.inc"
.include "../init/list/nsk_init_ppu_enable.inc"
.include "../init/list/nsk_init_sprites_draw.inc"
.include "../init/list/nsk_init_tiles_draw.inc"
.include "../init/list/nsk_init_ppu_flush.inc"

.segment "RODATA"

; @brief Init stages list
.export nsk_init_list
nsk_init_list:
    .addr nsk_init_ppu_disable
    .addr nsk_init_tiles_draw
    .addr nsk_init_sprites_draw
    .addr nsk_init_ppu_enable
    .addr nsk_init_ppu_flush
    .addr $0000

.endif