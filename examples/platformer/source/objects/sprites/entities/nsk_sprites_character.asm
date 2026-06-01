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
.include "nsk_sprites_whirl.inc"
.include "../../tiles/maps/nsk_map_vars.inc"

nsk_constructor _init

.segment "RODATA"

; @brief Character settings
.scope CHARACTER
    ; @brief Width of the character metasprite in hardware sprites
    WIDTH = 2

    ; @brief Height of the character metasprite in hardware sprites
    HEIGHT = 3

    ; @brief Number of hardware sprites per character frame
    COUNT = WIDTH * HEIGHT

    ; @brief Character sprite palette
    PALETTE = %10

    ; @brief Object flags
    FLAGS = \
        POOL::FLAGS::GRAVITY | \
        POOL::FLAGS::VECTORS  | \
        POOL::FLAGS::COLLISION

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

    ; @brief Character collision box
    .scope COLLISION
        ; @brief Left foot probe offset
        LEFT_X = 0

        ; @brief Right foot probe offset
        RIGHT_X = CHARACTER::WIDTH * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEWIDTH - 1

        ; @brief Foot probe Y offset
        FOOT_Y = CHARACTER::HEIGHT * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEHEIGHT

        ; @brief Collision box height
        HEIGHT = FOOT_Y
    .endscope

    ; @brief Character patrol settings
    .scope PATROL
        ; @brief Left patrol edge high byte
        LEFT_HI = 0

        ; @brief Left patrol edge low byte
        LEFT_LO = 175

        ; @brief Right patrol edge high byte
        RIGHT_HI = 1

        ; @brief Right patrol edge low byte
        RIGHT_LO = 85

        ; @brief Walking speed low byte
        SPEED_LO = $00

        ; @brief Walking speed fractional byte
        SPEED_FRAC = $70

        ; @brief Negative walking speed low byte
        SPEED_NEG_LO = $ff

        ; @brief Negative walking speed fractional byte
        SPEED_NEG_FRAC = $90
    .endscope

    ; @brief Character jump settings
    .scope JUMP
        ; @brief Initial jump speed low byte
        SPEED_NEG_LO = $fb

        ; @brief Initial jump speed fractional byte
        SPEED_NEG_FRAC = $00
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
            END:

            SIZE = END - TABLE

            .assert SIZE = CHARACTER::COUNT, error, "Character jump frame size mismatch"
        .endscope
    .endscope

    ; @brief Character animation frame sequences
    .scope ANIMATION
        ; @brief Walking animation frames
        .scope WALK
            ; @brief Number of game frames per animation frame
            DURATION = 8

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = CHARACTER::STATE::WALK

            TABLE:
                .addr CHARACTER::FRAME::WALK_0::TABLE
                .addr CHARACTER::FRAME::WALK_1::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Crouching animation frames
        .scope CROUCH
            ; @brief Number of game frames per animation frame
            DURATION = 120

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = CHARACTER::STATE::WALK

            TABLE:
                .addr CHARACTER::FRAME::CROUCH_0::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Jumping animation frames
        .scope JUMP
            ; @brief Number of game frames per animation frame
            DURATION = 120

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = CHARACTER::STATE::WALK

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

        ; @brief Animation frame durations indexed by CHARACTER::STATE
        DURATION:
            .byte WALK::DURATION
            .byte CROUCH::DURATION
            .byte JUMP::DURATION
        DURATION_END:

        DURATION_COUNT = DURATION_END - DURATION

        .assert \
            DURATION_COUNT = CHARACTER::STATE::COUNT, \
            error,                                    \
            "Character animation duration table size mismatch"

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

        ; @brief Animation next states indexed by CHARACTER::STATE
        NEXT_STATE:
            .byte WALK::NEXT_STATE
            .byte CROUCH::NEXT_STATE
            .byte JUMP::NEXT_STATE
        NEXT_STATE_END:

        NEXT_STATE_COUNT = NEXT_STATE_END - NEXT_STATE

        .assert \
            NEXT_STATE_COUNT = CHARACTER::STATE::COUNT, \
            error,                                      \
            "Character animation next-state table size mismatch"
    .endscope
.endscope

.segment "ZEROPAGE"

; @brief Current animation frame sequence pointer
_character_animation_ptr:
    .res 2
; @brief Current character frame sprite table pointer
_character_frame_ptr:
    .res 2
; @brief Current collision map page pointer
_character_map_ptr:
    .res 2

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
; @brief Current pool index
_character_pool_index:
    .res 1
; @brief Current character sprite index
_character_sprite:
    .res 1
; @brief Current character sprite attributes
_character_attrs:
    .res 1
; @brief Current draw loop sprite index
_character_draw_index:
    .res 1
; @brief Current draw loop sprite source index
_character_source_index:
    .res 1
; @brief Current draw loop sprite column
_character_draw_col:
    .res 1
; @brief Current draw loop sprite row offset
_character_draw_row:
    .res 1
; @brief Current foot probe X high byte
_character_probe_x_hi:
    .res 1
; @brief Current foot probe X low byte
_character_probe_x_lo:
    .res 1
; @brief Current foot probe Y
_character_probe_y:
    .res 1
; @brief Current collision map cell index
_character_map_index:
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
    lda #CHARACTER::ANIMATION::WALK::DURATION
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

    lda #CHARACTER::DATA::FREE
    ldy nsk_pool_data_id, x
    sta _character_data_used, y

    pull a, y

    rts
.endproc

; @brief Stops character horizontal movement
.proc _character_walk_stop
    ldx _character_pool_index

    lda #0
    sta nsk_pool_vectorx_lo, x
    sta nsk_pool_vectorx_frac, x

    rts
.endproc

; @brief Starts character jump movement
;
; @param[in] Y the index of the character data slot
.proc _character_jump_start
    ldx _character_pool_index

    lda #CHARACTER::STATE::JUMP
    sta _character_data_state, y

    lda #0
    sta _character_data_frame, y
    sta nsk_pool_vectorx_lo, x
    sta nsk_pool_vectorx_frac, x

    lda #CHARACTER::ANIMATION::JUMP::DURATION
    sta _character_data_timer, y

    lda #CHARACTER::JUMP::SPEED_NEG_LO
    sta nsk_pool_vectory_lo, x
    lda #CHARACTER::JUMP::SPEED_NEG_FRAC
    sta nsk_pool_vectory_frac, x

    rts
.endproc

; @brief Sets character horizontal movement to the left
.proc _character_walk_left
    ldx _character_pool_index

    lda #CHARACTER::PATROL::SPEED_NEG_LO
    sta nsk_pool_vectorx_lo, x
    lda #CHARACTER::PATROL::SPEED_NEG_FRAC
    sta nsk_pool_vectorx_frac, x

    rts
.endproc

; @brief Sets character horizontal movement to the right
.proc _character_walk_right
    ldx _character_pool_index

    lda #CHARACTER::PATROL::SPEED_LO
    sta nsk_pool_vectorx_lo, x
    lda #CHARACTER::PATROL::SPEED_FRAC
    sta nsk_pool_vectorx_frac, x

    rts
.endproc

; @brief Snaps character X position to the left patrol edge
.proc _character_snap_left
    ldx _character_pool_index

    lda #CHARACTER::PATROL::LEFT_HI
    sta nsk_pool_worldx_hi, x
    lda #CHARACTER::PATROL::LEFT_LO
    sta nsk_pool_worldx_lo, x
    lda #0
    sta nsk_pool_worldx_frac, x

    rts
.endproc

; @brief Snaps character X position to the right patrol edge
.proc _character_snap_right
    ldx _character_pool_index

    lda #CHARACTER::PATROL::RIGHT_HI
    sta nsk_pool_worldx_hi, x
    lda #CHARACTER::PATROL::RIGHT_LO
    sta nsk_pool_worldx_lo, x
    lda #0
    sta nsk_pool_worldx_frac, x

    rts
.endproc

; @brief Checks whether the character has reached the current whirl
;
; @param[in] X the index of the character in the nsk_pool_*
.proc _character_whirl_reached_check
    lda #0
    sta nsk_pool_result

    jsr nsk_whirl_requestx

    lda nsk_pool_worldx_lo, x
    clc
    adc #(CHARACTER::WIDTH * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEWIDTH - 1)
    sta _character_probe_x_lo
    lda nsk_pool_worldx_hi, x
    adc #0
    sta _character_probe_x_hi

    sec
    lda _character_probe_x_lo
    sbc nsk_whirl_query_x_lo
    tay
    lda _character_probe_x_hi
    sbc nsk_whirl_query_x_hi
    bne done

    tya
    cmp #((CHARACTER::WIDTH + 2) * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEWIDTH - 1)
    bcs done

    reached:
        lda #$01
        sta nsk_pool_result

    done:
        rts
.endproc

; @brief Ticks character jump state
;
; @param[in] Y the index of the character data slot
.proc _character_tick_jump
    ldx _character_pool_index

    lda nsk_pool_vectory_lo, x
    bmi done

    jsr nsk_character_isonground

    lda nsk_pool_result
    beq done

    ldx _character_pool_index
    ldy nsk_pool_data_id, x

    lda #CHARACTER::STATE::WALK
    sta _character_data_state, y

    lda #0
    sta _character_data_frame, y

    lda #CHARACTER::ANIMATION::WALK::DURATION
    sta _character_data_timer, y

    lda _character_data_direction, y
    cmp #CHARACTER::DIRECTION::LEFT
    beq left

    jsr _character_walk_right
    rts

    left:
        jsr _character_walk_left

    done:
        rts
.endproc

; @brief Routine to tick character-specific behavior
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_character_tick
.proc nsk_character_tick
    push a, x, y

    stx _character_pool_index

    ldy nsk_pool_data_id, x

    lda _character_data_state, y
    cmp #CHARACTER::STATE::WALK
    beq walk

    cmp #CHARACTER::STATE::JUMP
    beq jump

    jsr _character_walk_stop
    jmp done

    walk:
        jsr _character_whirl_reached_check

        ldx _character_pool_index
        ldy nsk_pool_data_id, x

        lda nsk_pool_result
        beq :+
            jsr _character_jump_start
            jmp done
        :

        lda _character_data_direction, y
        cmp #CHARACTER::DIRECTION::LEFT
        beq walk_left

    walk_right:
        lda nsk_pool_worldx_hi, x
        cmp #CHARACTER::PATROL::RIGHT_HI
        bcc :+
        bne turn_left

        lda nsk_pool_worldx_lo, x
        cmp #CHARACTER::PATROL::RIGHT_LO
        bcs turn_left
    :
        jsr _character_walk_right
        jmp done

    walk_left:
        lda nsk_pool_worldx_hi, x
        cmp #CHARACTER::PATROL::LEFT_HI
        bcc turn_right
        bne :+

        lda nsk_pool_worldx_lo, x
        cmp #CHARACTER::PATROL::LEFT_LO
        bcc turn_right
        beq turn_right
    :
        jsr _character_walk_left
        jmp done

    turn_left:
        jsr _character_snap_right

        lda #CHARACTER::DIRECTION::LEFT
        sta _character_data_direction, y

        jsr _character_walk_left
        jmp done

    turn_right:
        jsr _character_snap_left

        lda #CHARACTER::DIRECTION::RIGHT
        sta _character_data_direction, y

        jsr _character_walk_right
        jmp done

    jump:
        jsr _character_tick_jump

    done:
        pull a, x, y

    rts
.endproc

; @brief Routine to return the character collision box size
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_character_getbox
.proc nsk_character_getbox
    lda #(CHARACTER::WIDTH * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEWIDTH)
    sta nsk_pool_box_width
    lda #(CHARACTER::HEIGHT * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEHEIGHT)
    sta nsk_pool_box_height

    rts
.endproc

; @brief Routine to handle character collision
;
; @param[in] X the index of the object in the nsk_pool_*
; @param[in] nsk_pool_collision_other other collided pool index
.export nsk_character_collision
.proc nsk_character_collision
    ldy nsk_pool_collision_other
    lda nsk_pool_object, y
    cmp #SPRITELIST::FALLINGSTAR
    bne done

    ldy nsk_pool_data_id, x

    lda #CHARACTER::STATE::CROUCH
    sta _character_data_state, y

    lda #0
    sta _character_data_frame, y
    sta nsk_pool_vectorx_lo, x
    sta nsk_pool_vectorx_frac, x

    lda #CHARACTER::ANIMATION::CROUCH::DURATION
    sta _character_data_timer, y

    done:
    rts
.endproc

; @brief Selects the collision map page for the current foot probe
.proc _character_map_page_select
    lda #<nsk_map_grid
    sta _character_map_ptr + 0
    lda #>nsk_map_grid
    sta _character_map_ptr + 1

    lda _character_probe_x_hi
    beq done

    lda _character_map_ptr + 0
    clc
    adc #<MAP::SCREEN::PAGE
    sta _character_map_ptr + 0
    lda _character_map_ptr + 1
    adc #>MAP::SCREEN::PAGE
    sta _character_map_ptr + 1

    done:
        rts
.endproc

; @brief Checks if the current foot probe touches a solid collision block
;
; @param[in] _character_probe_x_hi Foot probe X high byte
; @param[in] _character_probe_x_lo Foot probe X low byte
; @param[in] _character_probe_y    Foot probe Y position
; @param[out] A 0 if empty, 1 if solid
.proc _character_ground_probe
    lda _character_probe_x_hi
    cmp #MAP::SCREEN::PAGES
    bcs empty

    lda _character_probe_y
    cmp #NSK::SCREEN::HEIGHT
    bcs empty

    jsr _character_map_page_select

    lda _character_probe_y
    and #$f0
    sta _character_map_index

    lda _character_probe_x_lo
    lsr
    lsr
    lsr
    lsr
    clc
    adc _character_map_index
    tay

    lda (_character_map_ptr), y
    cmp #MAP::COLLISION::SOLID
    beq solid

    empty:
        lda #0
        rts

    solid:
        lda #1
        rts
.endproc

; @brief Sets the character Y position to the top of the current ground block
.proc _character_snap_ground
    ldx _character_pool_index

    lda _character_probe_y
    and #$f0
    sec
    sbc #CHARACTER::COLLISION::HEIGHT
    sta nsk_pool_worldy_lo, x

    lda #0
    sta nsk_pool_worldy_frac, x

    rts
.endproc

; @brief Sets the current foot probe X position from the object X and offset
;
; @param[in] A Foot probe X offset
.proc _character_probe_x_set
    ldx _character_pool_index

    clc
    adc nsk_pool_worldx_lo, x
    sta _character_probe_x_lo

    lda nsk_pool_worldx_hi, x
    adc #0
    sta _character_probe_x_hi

    rts
.endproc

; @brief Routine to check if the character stands on ground
;
; @param[in] X the index of the object in the nsk_pool_*
; @note Writes 0 or 1 to nsk_pool_result and may snap the character Y position
.export nsk_character_isonground
.proc nsk_character_isonground
    stx _character_pool_index

    lda nsk_pool_worldy_lo, x
    clc
    adc #CHARACTER::COLLISION::FOOT_Y
    bcs done
    sta _character_probe_y

    lda #CHARACTER::COLLISION::LEFT_X
    jsr _character_probe_x_set
    jsr _character_ground_probe
    bne grounded

    lda #CHARACTER::COLLISION::RIGHT_X
    jsr _character_probe_x_set
    jsr _character_ground_probe
    beq done

    grounded:
        jsr _character_snap_ground

        lda #1
        sta nsk_pool_result

    done:
        rts
.endproc

; @brief Ticks character animation state
;
; @param[in] Y the index of the character data slot
.proc _character_animation_tick
    lda _character_data_timer, y
    beq advance

    sec
    sbc #1
    sta _character_data_timer, y
    jmp done

    advance:
        lda _character_data_state, y
        tax

        lda _character_data_frame, y
        clc
        adc #1
        cmp CHARACTER::ANIMATION::FRAMES, x
        bcc set_frame

        lda CHARACTER::ANIMATION::NEXT_STATE, x
        sta _character_data_state, y
        tax

        lda #0

    set_frame:
        sta _character_data_frame, y

        lda CHARACTER::ANIMATION::DURATION, x
        sta _character_data_timer, y

    done:
        rts
.endproc

; @brief Selects the current character frame sprite table
;
; @param[in] Y the index of the character data slot
.proc _character_frame_select
    lda _character_data_state, y
    asl
    tax

    lda CHARACTER::ANIMATION::TABLE + 0, x
    sta _character_animation_ptr + 0
    lda CHARACTER::ANIMATION::TABLE + 1, x
    sta _character_animation_ptr + 1

    lda _character_data_frame, y
    asl
    tay

    lda (_character_animation_ptr), y
    sta _character_frame_ptr + 0
    iny
    lda (_character_animation_ptr), y
    sta _character_frame_ptr + 1

    rts
.endproc

; @brief Selects the current character sprite attributes
;
; @param[in] Y the index of the character data slot
.proc _character_attrs_select
    lda _character_data_direction, y
    cmp #CHARACTER::DIRECTION::RIGHT
    beq flipped

    lda #CHARACTER::ATTRS
    jmp done

    flipped:
        lda #CHARACTER::ATTRS_FLIPPED

    done:
        sta _character_attrs
    rts
.endproc

; @brief Selects the source sprite index for the current draw index
.proc _character_source_index_select
    lda _character_draw_index
    sta _character_source_index

    lda _character_attrs
    cmp #CHARACTER::ATTRS_FLIPPED
    bne done

    lda _character_draw_index
    sta _character_draw_col
    lda #0
    sta _character_draw_row

    column:
        lda _character_draw_col
        cmp #CHARACTER::WIDTH
        bcc column_done

        sec
        sbc #CHARACTER::WIDTH
        sta _character_draw_col

        lda _character_draw_row
        clc
        adc #CHARACTER::WIDTH
        sta _character_draw_row
        jmp column

    column_done:
        lda #CHARACTER::WIDTH - 1
        sec
        sbc _character_draw_col
        clc
        adc _character_draw_row
        sta _character_source_index

    done:
        rts
.endproc

; @brief Draws the selected character frame
.proc _character_frame_draw
    ldx _character_pool_index
    ldy #0

    loop:
        sty _character_draw_index

        jsr _character_source_index_select
        ldy _character_source_index
        lda (_character_frame_ptr), y
        sta _character_sprite
        ldy _character_draw_index

        clc
        lda nsk_pool_screenx
        adc CHARACTER::POSX::TABLE, y
        bcs next
        sta _character_screenx

        clc
        lda nsk_pool_worldy_lo, x
        adc CHARACTER::POSY::TABLE, y
        sta _character_screeny

        nsk_sprite_draw \
            { _character_sprite  }, \
            { _character_attrs   }, \
            { #CHARACTER::PALETTE }, \
            { _character_screenx }, \
            { _character_screeny }

        next:
        iny
        cpy #CHARACTER::COUNT
        bne loop

    rts
.endproc

; @brief Routine to draw the character
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_character_draw
.proc nsk_character_draw
    stx _character_pool_index

    ldy nsk_pool_data_id, x

    jsr _character_animation_tick

    ldx _character_pool_index
    ldy nsk_pool_data_id, x
    jsr _character_frame_select

    ldx _character_pool_index
    ldy nsk_pool_data_id, x
    jsr _character_attrs_select

    jsr _character_frame_draw

    done:
    
    rts
.endproc

.endif
