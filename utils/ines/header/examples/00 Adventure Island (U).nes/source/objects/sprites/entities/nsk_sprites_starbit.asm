; @file nsk_sprites_starbit.asm
; @brief Source module for the star bit sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_STARBIT_ASM__
::__NSK_SPRITES_STARBIT_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "nsk_sprites_starbit.inc"

.include "../draw/nsk_sprites_draw.inc"
.include "../pool/nsk_pool_add.inc"
.include "../pool/nsk_pool_attrs.inc"
.include "../pool/nsk_pool_settings.inc"
.include "../pool/nsk_pool_vars.inc"
.include "../nsk_sprites_list.inc"
.include "../../../utils/nsk_util_rand8.inc"
.include "../../tiles/maps/nsk_map_vars.inc"

nsk_constructor _init

.segment "RODATA"

; @brief Star bit settings
.scope STARBIT
    ; @brief Width of the star bit metasprite in hardware sprites
    WIDTH = 1

    ; @brief Height of the star bit metasprite in hardware sprites
    HEIGHT = 1

    ; @brief Number of hardware sprites per star bit frame
    COUNT = WIDTH * HEIGHT

    ; @brief Star bit sprite palette
    PALETTE = %10

    ; @brief Object flags while falling
    FLAGS = \
        POOL::FLAGS::GRAVITY | \
        POOL::FLAGS::VECTORS

    ; @brief Star bit sprite attributes
    ATTRS = \
        NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::NO        | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO

    ; @brief Star bit animation states
    .scope STATE
        FALLING = 0 ; @< Falling animation
        FALLEN  = 1 ; @< Grounded animation

        ; @brief Number of star bit animation states
        COUNT = 2
    .endscope

    ; @brief Star bit collision probes
    .scope COLLISION
        ; @brief Center foot probe offset
        CENTER_X = 4

        ; @brief Foot probe Y offset
        FOOT_Y = STARBIT::HEIGHT * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEHEIGHT

        ; @brief Collision box height
        HEIGHT = FOOT_Y
    .endscope

    ; @brief Star bit object-specific data storage
    .scope DATA
        ; @brief Maximum number of star bit object-specific data slots
        MAX = 4

        ; @brief Free data slot marker
        FREE = $ff

        ; @brief Used data slot marker
        USED = $00
    .endscope

    ; @brief Star bit burst spawn settings
    .scope BURST
        ; @brief Number of star bits in a burst
        COUNT = 4

        ; @brief Spawn X offsets from the source object
        OFFSET_X:
            .byte 4,6,10,12
        OFFSET_X_END:

        OFFSET_X_COUNT = OFFSET_X_END - OFFSET_X

        .assert OFFSET_X_COUNT = COUNT, error, "Star bit X offset table size mismatch"

        ; @brief Spawn Y offsets from the source object
        OFFSET_Y:
            .byte 8,4,4,8
        OFFSET_Y_END:

        OFFSET_Y_COUNT = OFFSET_Y_END - OFFSET_Y

        .assert OFFSET_Y_COUNT = COUNT, error, "Star bit Y offset table size mismatch"

        ; @brief Initial X vector low bytes
        VECTOR_X_LO:
            .byte $ff,$ff,$00,$00
        VECTOR_X_LO_END:

        VECTOR_X_LO_COUNT = VECTOR_X_LO_END - VECTOR_X_LO

        .assert VECTOR_X_LO_COUNT = COUNT, error, "Star bit X vector low table size mismatch"

        ; @brief Initial X vector fractional bytes
        VECTOR_X_FRAC:
            .byte $20,$a0,$60,$e0
        VECTOR_X_FRAC_END:

        VECTOR_X_FRAC_COUNT = VECTOR_X_FRAC_END - VECTOR_X_FRAC

        .assert VECTOR_X_FRAC_COUNT = COUNT, error, "Star bit X vector frac table size mismatch"

        ; @brief Initial Y vector low bytes
        VECTOR_Y_LO:
            .byte $fb,$fa,$fb,$fa
        VECTOR_Y_LO_END:

        VECTOR_Y_LO_COUNT = VECTOR_Y_LO_END - VECTOR_Y_LO

        .assert VECTOR_Y_LO_COUNT = COUNT, error, "Star bit Y vector low table size mismatch"

        ; @brief Initial Y vector fractional bytes
        VECTOR_Y_FRAC:
            .byte $80,$e0,$e0,$80
        VECTOR_Y_FRAC_END:

        VECTOR_Y_FRAC_COUNT = VECTOR_Y_FRAC_END - VECTOR_Y_FRAC

        .assert VECTOR_Y_FRAC_COUNT = COUNT, error, "Star bit Y vector frac table size mismatch"
    .endscope

    ; @brief Star bit lifetime settings
    .scope LIFETIME
        ; @brief Minimum fallen lifetime in frames
        MIN = 100

        ; @brief Maximum random delta accepted for lifetime
        RANGE = 151
    .endscope

    ; @brief Star bit animation frames
    .scope FRAME
        ; @brief First falling animation frame
        .scope FALLING_0
            TABLE:
                .byte $7d
            END:

            SIZE = END - TABLE

            .assert SIZE = STARBIT::COUNT, error, "Star bit falling 0 frame size mismatch"
        .endscope

        ; @brief Second falling animation frame
        .scope FALLING_1
            TABLE:
                .byte $7e
            END:

            SIZE = END - TABLE

            .assert SIZE = STARBIT::COUNT, error, "Star bit falling 1 frame size mismatch"
        .endscope

        ; @brief First fallen animation frame
        .scope FALLEN_0
            TABLE:
                .byte $8d
            END:

            SIZE = END - TABLE

            .assert SIZE = STARBIT::COUNT, error, "Star bit fallen 0 frame size mismatch"
        .endscope

        ; @brief Second fallen animation frame
        .scope FALLEN_1
            TABLE:
                .byte $8e
            END:

            SIZE = END - TABLE

            .assert SIZE = STARBIT::COUNT, error, "Star bit fallen 1 frame size mismatch"
        .endscope
    .endscope

    ; @brief Star bit animation frame sequences
    .scope ANIMATION
        ; @brief Falling animation frames
        .scope FALLING
            ; @brief Number of game frames per animation frame
            DURATION = 4

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = STARBIT::STATE::FALLING

            TABLE:
                .addr STARBIT::FRAME::FALLING_0::TABLE
                .addr STARBIT::FRAME::FALLING_1::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Fallen animation frames
        .scope FALLEN
            ; @brief Number of game frames per animation frame
            DURATION = 8

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = STARBIT::STATE::FALLEN

            TABLE:
                .addr STARBIT::FRAME::FALLEN_0::TABLE
                .addr STARBIT::FRAME::FALLEN_1::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Animation frame tables indexed by STARBIT::STATE
        TABLE:
            .addr FALLING::TABLE
            .addr FALLEN::TABLE
        END:

        COUNT = (END - TABLE) / 2

        .assert COUNT = STARBIT::STATE::COUNT, error, "Star bit animation table size mismatch"

        ; @brief Animation frame durations indexed by STARBIT::STATE
        DURATION:
            .byte FALLING::DURATION
            .byte FALLEN::DURATION
        DURATION_END:

        DURATION_COUNT = DURATION_END - DURATION

        .assert \
            DURATION_COUNT = STARBIT::STATE::COUNT, \
            error,                                  \
            "Star bit animation duration table size mismatch"

        ; @brief Number of frames per animation indexed by STARBIT::STATE
        FRAMES:
            .byte FALLING::COUNT
            .byte FALLEN::COUNT
        FRAMES_END:

        FRAMES_COUNT = FRAMES_END - FRAMES

        .assert \
            FRAMES_COUNT = STARBIT::STATE::COUNT, \
            error,                                \
            "Star bit animation frame-count table size mismatch"

        ; @brief Animation next states indexed by STARBIT::STATE
        NEXT_STATE:
            .byte FALLING::NEXT_STATE
            .byte FALLEN::NEXT_STATE
        NEXT_STATE_END:

        NEXT_STATE_COUNT = NEXT_STATE_END - NEXT_STATE

        .assert \
            NEXT_STATE_COUNT = STARBIT::STATE::COUNT, \
            error,                                    \
            "Star bit animation next-state table size mismatch"
    .endscope
.endscope

.segment "ZEROPAGE"

; @brief Current animation frame sequence pointer
_starbit_animation_ptr:
    .res 2
; @brief Current star bit frame sprite table pointer
_starbit_frame_ptr:
    .res 2

.segment "BSS"

; @brief Allocated star bit data slot
_starbit_data_id:
    .res 1
; @brief Current pool index
_starbit_pool_index:
    .res 1
; @brief Source pool index for spawn
_starbit_spawn_source:
    .res 1
; @brief Current burst index
_starbit_burst_index:
    .res 1
; @brief Prepared spawn X high byte
_starbit_spawn_worldx_hi:
    .res 1
; @brief Prepared spawn X low byte
_starbit_spawn_worldx_lo:
    .res 1
; @brief Prepared spawn Y
_starbit_spawn_worldy_lo:
    .res 1
; @brief Current star bit sprite index
_starbit_sprite:
    .res 1
; @brief Current star bit sprite Y position
_starbit_screeny:
    .res 1
; @brief Current foot probe X high byte
_starbit_probe_x_hi:
    .res 1
; @brief Current foot probe X low byte
_starbit_probe_x_lo:
    .res 1
; @brief Current foot probe Y
_starbit_probe_y:
    .res 1
; @brief Current collision map cell index
_starbit_map_index:
    .res 1

; @brief Star bit data slot usage markers
_starbit_data_used:
    .res STARBIT::DATA::MAX
; @brief Star bit current state per data slot
_starbit_data_state:
    .res STARBIT::DATA::MAX
; @brief Star bit animation frame index per data slot
_starbit_data_frame:
    .res STARBIT::DATA::MAX
; @brief Star bit animation timer per data slot
_starbit_data_timer:
    .res STARBIT::DATA::MAX

.segment "CODE"

; @brief Initializes the star bit object-specific data pool
.proc _init
    push a, x

    lda #STARBIT::DATA::FREE
    ldx #0

    loop:
        sta _starbit_data_used, x

        inx
        cpx #STARBIT::DATA::MAX
        bne loop

    pull a, x

    rts
.endproc

; @brief Allocates one star bit object-specific data slot
;
; @param[out] X data slot index
; @param[out] C clear if allocated, set if no free slots are available
.proc _starbit_data_alloc
    ldx #0

    loop:
        lda _starbit_data_used, x
        cmp #STARBIT::DATA::FREE
        beq found

        inx
        cpx #STARBIT::DATA::MAX
        bne loop

    sec
    rts

    found:
        clc
        rts
.endproc

; @brief Starts the falling state for a data slot
;
; @param[in] Y the index of the star bit data slot
.proc _starbit_data_start_falling
    lda #STARBIT::DATA::USED
    sta _starbit_data_used, y

    lda #STARBIT::STATE::FALLING
    sta _starbit_data_state, y

    jsr nsk_util_rand8
    and %1
    sta _starbit_data_frame, y

    lda #STARBIT::ANIMATION::FALLING::DURATION
    sta _starbit_data_timer, y

    rts
.endproc

; @brief Prepares randomized fallen lifetime
;
; @param[out] A lifetime in frames, in range 100..250
.proc _starbit_lifetime_prepare
    retry:
        jsr nsk_util_rand8
        cmp #STARBIT::LIFETIME::RANGE
        bcs retry

    clc
    adc #STARBIT::LIFETIME::MIN

    rts
.endproc

; @brief Prepares one star bit spawn position from the current burst index
.proc _starbit_spawn_position_prepare
    ldx _starbit_spawn_source
    ldy _starbit_burst_index

    clc
    lda nsk_pool_worldx_lo, x
    adc STARBIT::BURST::OFFSET_X, y
    sta _starbit_spawn_worldx_lo

    lda nsk_pool_worldx_hi, x
    adc #0
    sta _starbit_spawn_worldx_hi

    clc
    lda nsk_pool_worldy_lo, x
    adc STARBIT::BURST::OFFSET_Y, y
    sta _starbit_spawn_worldy_lo

    rts
.endproc

; @brief Spawns one star bit for the current burst index
.proc _starbit_spawn
    lda nsk_pool_size
    cmp #_NSK_POOL_MAX
    beq done

    jsr _starbit_data_alloc
    bcs done

    stx _starbit_data_id

    jsr _starbit_spawn_position_prepare

    ldy _starbit_data_id
    jsr _starbit_data_start_falling

    ldy _starbit_burst_index

    nsk_pool_add \
        { #SPRITELIST::STARBIT              }, \
        { #STARBIT::FLAGS                   }, \
        { _starbit_spawn_worldx_hi          }, \
        { _starbit_spawn_worldx_lo          }, \
        { _starbit_spawn_worldy_lo          }, \
        { STARBIT::BURST::VECTOR_X_LO, y    }, \
        { STARBIT::BURST::VECTOR_X_FRAC, y  }, \
        { STARBIT::BURST::VECTOR_Y_LO, y    }, \
        { STARBIT::BURST::VECTOR_Y_FRAC, y  }, \
        { #0                                }, \
        { _starbit_data_id                  }

    done:
        rts
.endproc

; @brief Spawns a burst of star bits at the source pool object coordinates
;
; @param[in] X source pool object index
.export nsk_starbit_spawn_burst
.proc nsk_starbit_spawn_burst
    push a, x, y

    stx _starbit_spawn_source

    ldy #0

    loop:
        sty _starbit_burst_index

        jsr _starbit_spawn

        ldy _starbit_burst_index
        iny
        cpy #STARBIT::BURST::COUNT
        bne loop

    pull a, x, y

    rts
.endproc

; @brief Routine to clean up star bit object-specific data
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_starbit_clean
.proc nsk_starbit_clean
    ldy nsk_pool_data_id, x

    lda #STARBIT::DATA::FREE
    sta _starbit_data_used, y

    rts
.endproc

; @brief Checks if the current foot probe touches a solid collision block
;
; @param[in] _starbit_probe_x_hi Foot probe X high byte
; @param[in] _starbit_probe_x_lo Foot probe X low byte
; @param[in] _starbit_probe_y    Foot probe Y position
; @param[out] A 0 if empty, 1 if solid
.proc _starbit_ground_probe
    ; These sanity checks are useless for our case
    ; as we guarantee that the starbits won't fly
    ; far enough for this.
    ;
    ;lda _starbit_probe_x_hi
    ;cmp #MAP::SCREEN::PAGES
    ;bcs empty
    ;
    ;lda _starbit_probe_y
    ;cmp #NSK::SCREEN::HEIGHT
    ;bcs empty

    lda _starbit_probe_y
    and #$f0
    sta _starbit_map_index

    lda _starbit_probe_x_lo
    lsr
    lsr
    lsr
    lsr
    clc
    adc _starbit_map_index
    tay

    ; Fast path for the current 2-page map layout. Adapt this if collision
    ; data becomes streamed or MAP::SCREEN::PAGES grows beyond two pages.
    lda _starbit_probe_x_hi
    beq :+
        lda nsk_map_grid + MAP::SCREEN::PAGE, y
        jmp check
    :
        lda nsk_map_grid, y

    check:
    cmp #MAP::COLLISION::SOLID
    beq solid

    empty:
        lda #0
        rts

    solid:
        lda #1
        rts
.endproc

; @brief Sets the star bit Y position to the top of the current ground block
.proc _starbit_snap_ground
    ldx _starbit_pool_index

    lda _starbit_probe_y
    and #$f0
    sec
    sbc #STARBIT::COLLISION::HEIGHT
    sta nsk_pool_worldy_lo, x

    lda #0
    sta nsk_pool_worldy_frac, x

    rts
.endproc

; @brief Sets the current foot probe X position from the object X and offset
;
; @param[in] A Foot probe X offset
.proc _starbit_probe_x_set
    ldx _starbit_pool_index

    clc
    adc nsk_pool_worldx_lo, x
    sta _starbit_probe_x_lo

    lda nsk_pool_worldx_hi, x
    adc #0
    sta _starbit_probe_x_hi

    rts
.endproc

; @brief Starts the grounded fallen state for the current star bit
;
; @param[in] Y the index of the star bit data slot
.proc _starbit_state_fallen
    lda #STARBIT::STATE::FALLEN
    sta _starbit_data_state, y

    lda #0
    sta _starbit_data_frame, y

    lda #STARBIT::ANIMATION::FALLEN::DURATION
    sta _starbit_data_timer, y

    ldx _starbit_pool_index

    lda #0
    sta nsk_pool_vectorx_lo, x
    sta nsk_pool_vectorx_frac, x
    sta nsk_pool_vectory_lo, x
    sta nsk_pool_vectory_frac, x

    lda nsk_pool_flags, x
    and #($ff - POOL::FLAGS::GRAVITY - POOL::FLAGS::VECTORS)
    sta nsk_pool_flags, x

    jsr _starbit_lifetime_prepare
    sta nsk_pool_timer_a, x

    rts
.endproc

; @brief Marks the current star bit for removal
.proc _starbit_delete
    ldx _starbit_pool_index

    lda nsk_pool_flags, x
    ora #POOL::FLAGS::DELETED
    sta nsk_pool_flags, x

    rts
.endproc

; @brief Routine to check whether star bit touches ground
;
; @param[in] X the index of the object in the nsk_pool_*
; @param[out] nsk_pool_result 0 if empty, non-zero if solid
.export nsk_starbit_isonground
.proc nsk_starbit_isonground
    stx _starbit_pool_index

    lda nsk_pool_worldy_lo, x
    
    ; This sanity check isn't needed in our case
    ;
    ;clc
    ;adc #STARBIT::COLLISION::FOOT_Y
    ;bcs done

    sta _starbit_probe_y

    lda #STARBIT::COLLISION::CENTER_X
    jsr _starbit_probe_x_set
    jsr _starbit_ground_probe
    beq done

    grounded:
        jsr _starbit_snap_ground

        ldx _starbit_pool_index
        ldy nsk_pool_data_id, x
        jsr _starbit_state_fallen

        lda #1
        sta nsk_pool_result

    done:
        rts
.endproc

; @brief Ticks star bit animation state
;
; @param[in] Y the index of the star bit data slot
.proc _starbit_animation_tick
    lda _starbit_data_timer, y
    beq advance

    sec
    sbc #1
    sta _starbit_data_timer, y
    jmp done

    advance:
        lda _starbit_data_state, y
        tax

        lda _starbit_data_frame, y
        clc
        adc #1
        cmp STARBIT::ANIMATION::FRAMES, x
        bcc set_frame

        lda STARBIT::ANIMATION::NEXT_STATE, x
        sta _starbit_data_state, y
        tax

        lda #0

    set_frame:
        sta _starbit_data_frame, y

        lda STARBIT::ANIMATION::DURATION, x
        sta _starbit_data_timer, y

    done:
        rts
.endproc

; @brief Ticks the current fallen star bit lifetime
.proc _starbit_fallen_tick
    ldx _starbit_pool_index

    lda nsk_pool_timer_a, x
    beq delete

    sec
    sbc #1
    sta nsk_pool_timer_a, x
    bne done

    delete:
        jsr _starbit_delete

    done:
        rts
.endproc

; @brief Routine to tick star bit-specific behavior
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_starbit_tick
.proc nsk_starbit_tick
    push a, x, y

    stx _starbit_pool_index

    ldy nsk_pool_data_id, x
    jsr _starbit_animation_tick

    ldx _starbit_pool_index
    ldy nsk_pool_data_id, x
    lda _starbit_data_state, y
    cmp #STARBIT::STATE::FALLEN
    bne done

    jsr _starbit_fallen_tick

    done:
        pull a, x, y

    rts
.endproc

; @brief Selects the current star bit frame sprite table
;
; @param[in] Y the index of the star bit data slot
.proc _starbit_frame_select
    lda _starbit_data_state, y
    asl
    tax

    lda STARBIT::ANIMATION::TABLE + 0, x
    sta _starbit_animation_ptr + 0
    lda STARBIT::ANIMATION::TABLE + 1, x
    sta _starbit_animation_ptr + 1

    lda _starbit_data_frame, y
    asl
    tay

    lda (_starbit_animation_ptr), y
    sta _starbit_frame_ptr + 0
    iny
    lda (_starbit_animation_ptr), y
    sta _starbit_frame_ptr + 1

    rts
.endproc

; @brief Draws the selected star bit frame
.proc _starbit_frame_draw
    ldx _starbit_pool_index

    ldy #0
    lda (_starbit_frame_ptr), y
    sta _starbit_sprite

    lda nsk_pool_worldy_lo, x
    sta _starbit_screeny

    nsk_sprite_draw \
        { _starbit_sprite     }, \
        { #STARBIT::ATTRS     }, \
        { #STARBIT::PALETTE   }, \
        { nsk_pool_screenx    }, \
        { _starbit_screeny    }

    rts
.endproc

; @brief Routine to draw the star bit
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_starbit_draw
.proc nsk_starbit_draw
    stx _starbit_pool_index

    ldy nsk_pool_data_id, x
    jsr _starbit_frame_select

    jsr _starbit_frame_draw

    rts
.endproc

.endif
