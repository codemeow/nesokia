; @file nsk_sprites_character.asm
; @brief Source module for the character sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_CHARACTER_ASM__
::__NSK_SPRITES_CHARACTER_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "nsk_sprites_character.inc"

.include "../draw/nsk_sprites_draw.inc"
.include "../pool/nsk_pool_add.inc"
.include "../pool/nsk_pool_attrs.inc"
.include "../pool/nsk_pool_settings.inc"
.include "../pool/nsk_pool_vars.inc"
.include "../nsk_sprites_list.inc"

nsk_constructor _init

.segment "RODATA"

; @brief Character settings
.scope CHARACTER
    ; @brief Width of the character metasprite in hardware sprites
    WIDTH = 2

    ; @brief Height of the character metasprite in hardware sprites
    HEIGHT = 4

    ; @brief Number of hardware sprites per character frame
    COUNT = WIDTH * HEIGHT

    ; @brief Character sprite palette
    PALETTE = %10

    ; @brief Object flags
    FLAGS = 0

    ; @brief Character start X high byte
    STARTX_HI = 0

    ; @brief Character start X low byte
    STARTX_LO = 216

    ; @brief Character start Y
    STARTY = 167

    ; @brief Base character sprite attributes
    ATTRS = \
        NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::NO        | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO

    ; @brief Horizontally flipped character sprite attributes
    ATTRS_FLIPPED = \
        NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::YES       | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO

    ; @brief Character frame X offsets
    .scope POSX
        TABLE:
            .byte 0,8
            .byte 0,8
            .byte 0,8
            .byte 0,8
        END:

        SIZE = END - TABLE

        .assert SIZE = CHARACTER::COUNT, error, "Character X offset table size mismatch"
    .endscope

    ; @brief Character frame Y offsets
    .scope POSY
        TABLE:
            .byte 0,0
            .byte 8,8
            .byte 16,16
            .byte 24,24
        END:

        SIZE = END - TABLE

        .assert SIZE = CHARACTER::COUNT, error, "Character Y offset table size mismatch"
    .endscope

    ; @brief Character animation states
    .scope STATE
        WALK   = 0 ; @< Walking animation
        CROUCH = 1 ; @< Crouching animation
        JUMP   = 2 ; @< Jumping animation

        ; @brief Number of character animation states
        COUNT = 3
    .endscope

    ; @brief Character direction states
    .scope DIRECTION
        LEFT  = 0 ; @< Looking left
        RIGHT = 1 ; @< Looking right
    .endscope

    ; @brief Character object-specific data storage
    .scope DATA
        ; @brief Maximum number of character object-specific data slots
        MAX = 1

        ; @brief Free data slot marker
        FREE = $ff

        ; @brief Used data slot marker
        USED = $00
    .endscope

    ; @brief Character animation frames
    .scope FRAME
        ; @brief First walking animation frame
        .scope WALK_0
            TABLE:
                .byte $48,$49
                .byte $58,$59
                .byte $68,$69
                .byte $78,$79
            END:

            SIZE = END - TABLE

            .assert SIZE = CHARACTER::COUNT, error, "Character walk 0 frame size mismatch"
        .endscope

        ; @brief Second walking animation frame
        .scope WALK_1
            TABLE:
                .byte $4a,$4b
                .byte $5a,$5b
                .byte $6a,$6b
                .byte $7a,$7b
            END:

            SIZE = END - TABLE

            .assert SIZE = CHARACTER::COUNT, error, "Character walk 1 frame size mismatch"
        .endscope

        ; @brief Crouching animation frame
        .scope CROUCH_0
            TABLE:
                .byte $4c,$4d
                .byte $5c,$5d
                .byte $6c,$6d
                .byte $7c,$7d
            END:

            SIZE = END - TABLE

            .assert SIZE = CHARACTER::COUNT, error, "Character crouch frame size mismatch"
        .endscope

        ; @brief Jumping animation frame
        .scope JUMP_0
            TABLE:
                .byte $4e,$4f
                .byte $5e,$5f
                .byte $6e,$6f
                .byte $7e,$7f
            END:

            SIZE = END - TABLE

            .assert SIZE = CHARACTER::COUNT, error, "Character jump frame size mismatch"
        .endscope
    .endscope

    ; @brief Character animation frame sequences
    .scope ANIMATION
        ; @brief Walking animation frames
        .scope WALK
            TABLE:
                .addr CHARACTER::FRAME::WALK_0::TABLE
                .addr CHARACTER::FRAME::WALK_1::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Crouching animation frames
        .scope CROUCH
            TABLE:
                .addr CHARACTER::FRAME::CROUCH_0::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Jumping animation frames
        .scope JUMP
            TABLE:
                .addr CHARACTER::FRAME::JUMP_0::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Animation frame tables indexed by CHARACTER::STATE
        TABLE:
            .addr WALK::TABLE
            .addr CROUCH::TABLE
            .addr JUMP::TABLE
        END:

        COUNT = (END - TABLE) / 2

        .assert COUNT = CHARACTER::STATE::COUNT, error, "Character animation table size mismatch"

        ; @brief Number of frames per animation indexed by CHARACTER::STATE
        FRAMES:
            .byte WALK::COUNT
            .byte CROUCH::COUNT
            .byte JUMP::COUNT
        FRAMES_END:

        FRAMES_COUNT = FRAMES_END - FRAMES

        .assert \
            FRAMES_COUNT = CHARACTER::STATE::COUNT, \
            error,                                  \
            "Character animation frame-count table size mismatch"
    .endscope
.endscope

.segment "BSS"

; @brief Current character sprite X position
_character_screenx:
    .res 1
; @brief Current character sprite Y position
_character_screeny:
    .res 1
; @brief Allocated character data slot
_character_data_id:
    .res 1

; @brief Character data slot usage markers
_character_data_used:
    .res CHARACTER::DATA::MAX
; @brief Character current state per data slot
_character_data_state:
    .res CHARACTER::DATA::MAX
; @brief Character direction per data slot
_character_data_direction:
    .res CHARACTER::DATA::MAX
; @brief Character animation frame index per data slot
_character_data_frame:
    .res CHARACTER::DATA::MAX
; @brief Character animation timer per data slot
_character_data_timer:
    .res CHARACTER::DATA::MAX

.segment "CODE"

; @brief Initializes the character object-specific data pool
.proc _init
    push a, x

    lda #CHARACTER::DATA::FREE
    ldx #0

    loop:
        sta _character_data_used, x

        inx
        cpx #CHARACTER::DATA::MAX
        bne loop

    pull a, x

    rts
.endproc

; @brief Allocates one character object-specific data slot
;
; @param[out] X data slot index
; @param[out] C clear if allocated, set if no free slots are available
.proc _character_data_alloc
    ldx #0

    loop:
        lda _character_data_used, x
        cmp #CHARACTER::DATA::FREE
        beq found

        inx
        cpx #CHARACTER::DATA::MAX
        bne loop

    sec
    rts

    found:
        clc
        rts
.endproc

; @brief Inits the character in the object pool
.export nsk_character_init
.proc nsk_character_init
    push a, x

    lda nsk_pool_size
    cmp #_NSK_POOL_MAX
    beq done

    jsr _character_data_alloc
    bcs done

    stx _character_data_id

    lda #CHARACTER::DATA::USED
    sta _character_data_used, x
    lda #CHARACTER::STATE::WALK
    sta _character_data_state, x
    lda #CHARACTER::DIRECTION::LEFT
    sta _character_data_direction, x
    lda #0
    sta _character_data_frame, x
    sta _character_data_timer, x

    nsk_pool_add \
        { #SPRITELIST::CHARACTER    }, \
        { #CHARACTER::FLAGS         }, \
        { #CHARACTER::STARTX_HI     }, \
        { #CHARACTER::STARTX_LO     }, \
        { #CHARACTER::STARTY        }, \
        { #0                        }, \
        { #0                        }, \
        { #0                        }, \
        { #0                        }, \
        { #0                        }, \
        { _character_data_id        }

    done:
        pull a, x

    rts
.endproc

; @brief Routine to handle out-of-bounds character behavior
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_character_oob
.proc nsk_character_oob
    rts
.endproc

; @brief Routine to clean up character object-specific data
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_character_clean
.proc nsk_character_clean
    push a, y

    ldy nsk_pool_data_id, x
    cpy #CHARACTER::DATA::MAX
    bcs done

    lda #CHARACTER::DATA::FREE
    sta _character_data_used, y

    done:
        pull a, y

    rts
.endproc

; @brief Routine to draw the character
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_character_draw
.proc nsk_character_draw
    push a, y

    ldy #0

    loop:
        clc
        lda nsk_pool_screenx
        adc CHARACTER::POSX::TABLE, y
        sta _character_screenx

        clc
        lda nsk_pool_worldy_lo, x
        adc CHARACTER::POSY::TABLE, y
        sta _character_screeny

        nsk_sprite_draw \
            { CHARACTER::FRAME::WALK_0::TABLE, y }, \
            { #CHARACTER::ATTRS                  }, \
            { #CHARACTER::PALETTE                }, \
            { _character_screenx                 }, \
            { _character_screeny                 }

        iny
        cpy #CHARACTER::COUNT
        bne loop

    pull a, y

    rts
.endproc

.endif
