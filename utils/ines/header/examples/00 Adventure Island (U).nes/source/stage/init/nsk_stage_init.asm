; @file nsk_init_loop.asm
; @brief Source module for the init functions loop runner
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_STAGE_INIT_ASM__
::__NSK_STAGE_INIT_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "nsk_stage_init.inc"

.include "list/nsk_init_ppu_disable.inc"
.include "list/nsk_init_ppu_enable.inc"
.include "list/nsk_init_sprites_draw.inc"
.include "list/nsk_init_tiles_draw.inc"
.include "list/nsk_init_ppu_flush.inc"

.include "../../objects/sprites/nsk_sprites_list.inc"
.include "../nsk_stage_control.inc"

.segment "RODATA"

; @brief List of initialization routines
.scope INITS
    TABLE:
        .addr nsk_init_ppu_disable

        .addr nsk_hud_init
        .addr nsk_debris_init

        .addr nsk_init_tiles_draw
        .addr nsk_init_sprites_draw
        .addr nsk_init_ppu_enable
        .addr nsk_init_ppu_flush
    _TABLE_END:

    COUNT = (INITS::_TABLE_END - INITS::TABLE) / 2
.endscope

.segment "BSS"

; @brief Jumper pointer
_init_jumper:
    .res 2

.segment "CODE"

; @brief Inits all the stages routines
.export nsk_stage_init
.proc nsk_stage_init
    push a,x

    ldx #0

    loop:
        jai \
            _init_jumper, \
            INITS::TABLE, \
            x

        inx
        cpx #INITS::COUNT
        bne loop

    lda #GAMESTAGE_START
    sta game_stage_num

    pull a,x

    rts
.endproc

.endif
