; @file nsk_sprites_fallingstar.asm
; @brief Source module for the falling star sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_FALLINGSTAR_ASM__
::__NSK_SPRITES_FALLINGSTAR_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "nsk_sprites_fallingstar.inc"

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

; @brief Falling star settings
.scope FALLINGSTAR
    ; @brief Width of the falling metasprite in hardware sprites
    WIDTH = 2

    ; @brief Height of the falling metasprite in hardware sprites
    HEIGHT = 2

    ; @brief Number of hardware sprites per falling frame
    COUNT = WIDTH * HEIGHT

    ; @brief Width of the fallen metasprite in hardware sprites
    FALLEN_WIDTH = 2

    ; @brief Height of the fallen metasprite in hardware sprites
    FALLEN_HEIGHT = 1

    ; @brief Number of hardware sprites per fallen frame
    FALLEN_COUNT = FALLEN_WIDTH * FALLEN_HEIGHT

    ; @brief Falling star sprite palette
    PALETTE = %10

    ; @brief Object flags
    FLAGS = \
        POOL::FLAGS::GRAVITY | \
        POOL::FLAGS::VECTORS

    ; @brief Base falling star sprite attributes
    ATTRS = \
        NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::NO        | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO

    ; @brief Horizontally flipped falling star sprite attributes
    ATTRS_FLIP_HOR = \
        NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::YES       | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO

    ; @brief Vertically flipped falling star sprite attributes
    ATTRS_FLIP_VER = \
        NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::NO        | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::YES

    ; @brief Horizontally and vertically flipped falling star sprite attributes
    ATTRS_FLIP_BOTH = \
        NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::YES       | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::YES

    ; @brief Falling star frame X offsets
    .scope POSX
        TABLE:
            .byte 0,8
            .byte 0,8
        END:

        SIZE = END - TABLE

        .assert SIZE = FALLINGSTAR::COUNT, error, "Falling star X offset table size mismatch"
    .endscope

    ; @brief Falling star frame Y offsets
    .scope POSY
        TABLE:
            .byte 0,0
            .byte 8,8
        END:

        SIZE = END - TABLE

        .assert SIZE = FALLINGSTAR::COUNT, error, "Falling star Y offset table size mismatch"
    .endscope

    ; @brief Falling star first animation frame sprite attributes per source sprite
    .scope DRAW_ATTRS
        TABLE:
            .byte FALLINGSTAR::ATTRS
            .byte FALLINGSTAR::ATTRS_FLIP_HOR
            .byte FALLINGSTAR::ATTRS_FLIP_VER
            .byte FALLINGSTAR::ATTRS_FLIP_BOTH
        END:

        SIZE = END - TABLE

        .assert SIZE = FALLINGSTAR::COUNT, error, "Falling star attrs table size mismatch"
    .endscope

    ; @brief Falling star second animation frame sprite attributes per source sprite
    .scope DRAW_ATTRS_1
        TABLE:
            .byte FALLINGSTAR::ATTRS_FLIP_VER
            .byte FALLINGSTAR::ATTRS_FLIP_BOTH
            .byte FALLINGSTAR::ATTRS
            .byte FALLINGSTAR::ATTRS_FLIP_HOR
        END:

        SIZE = END - TABLE

        .assert SIZE = FALLINGSTAR::COUNT, error, "Falling star second attrs table size mismatch"
    .endscope

    ; @brief Falling star animation states
    .scope STATE
        FALLING = 0 ; @< Falling animation
        FALLEN  = 1 ; @< Grounded animation

        ; @brief Number of falling star animation states
        COUNT = 2
    .endscope

    ; @brief Falling star collision probes
    .scope COLLISION
        ; @brief Center foot probe offset
        CENTER_X = 8

        ; @brief Foot probe Y offset while falling
        FOOT_Y = FALLINGSTAR::HEIGHT * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEHEIGHT

        ; @brief Collision box height while falling
        HEIGHT = FOOT_Y
    .endscope

    ; @brief Falling star object-specific data storage
    .scope DATA
        ; @brief Maximum number of falling star object-specific data slots
        MAX = 2

        ; @brief Free data slot marker
        FREE = $ff

        ; @brief Used data slot marker
        USED = $00
    .endscope

    ; @brief Falling star initial movement vectors
    .scope VECTOR
        ; @brief Initial X vector low byte
        X_LO = 2

        ; @brief Initial X vector fractional byte
        X_FRAC = 0

        ; @brief Initial Y vector low byte
        Y_LO = 252

        ; @brief Initial Y vector fractional byte
        Y_FRAC = 0
    .endscope

    ; @brief Falling star WHIRL respawn settings
    .scope RESPAWN
        ; @brief Number of frames before spawning a new WHIRL
        TIMER = 250
    .endscope

    ; @brief Falling star animation frames
    .scope FRAME
        ; @brief First falling animation frame
        .scope FALLING_0
            TABLE:
                .byte $7f,$7f
                .byte $7f,$7f
            END:

            SIZE = END - TABLE

            .assert SIZE = FALLINGSTAR::COUNT, error, "Falling star falling 0 frame size mismatch"
        .endscope

        ; @brief Second falling animation frame
        .scope FALLING_1
            TABLE:
                .byte $8f,$8f
                .byte $8f,$8f
            END:

            SIZE = END - TABLE

            .assert SIZE = FALLINGSTAR::COUNT, error, "Falling star falling 1 frame size mismatch"
        .endscope

        ; @brief First fallen animation frame
        .scope FALLEN_0
            TABLE:
                .byte $7f,$7f
            END:

            SIZE = END - TABLE

            .assert SIZE = FALLINGSTAR::FALLEN_COUNT, error, "Falling star fallen 0 frame size mismatch"
        .endscope

        ; @brief Second fallen animation frame
        .scope FALLEN_1
            TABLE:
                .byte $8f,$8f
            END:

            SIZE = END - TABLE

            .assert SIZE = FALLINGSTAR::FALLEN_COUNT, error, "Falling star fallen 1 frame size mismatch"
        .endscope
    .endscope

    ; @brief Falling star animation frame sequences
    .scope ANIMATION
        ; @brief Falling animation frames
        .scope FALLING
            ; @brief Number of game frames per animation frame
            DURATION = 1

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = FALLINGSTAR::STATE::FALLING

            TABLE:
                .addr FALLINGSTAR::FRAME::FALLING_0::TABLE
                .addr FALLINGSTAR::FRAME::FALLING_1::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Fallen animation frames
        .scope FALLEN
            ; @brief Number of game frames per animation frame
            DURATION = 2

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = FALLINGSTAR::STATE::FALLEN

            TABLE:
                .addr FALLINGSTAR::FRAME::FALLEN_0::TABLE
                .addr FALLINGSTAR::FRAME::FALLEN_1::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Animation frame tables indexed by FALLINGSTAR::STATE
        TABLE:
            .addr FALLING::TABLE
            .addr FALLEN::TABLE
        END:

        COUNT = (END - TABLE) / 2

        .assert COUNT = FALLINGSTAR::STATE::COUNT, error, "Falling star animation table size mismatch"

        ; @brief Animation frame durations indexed by FALLINGSTAR::STATE
        DURATION:
            .byte FALLING::DURATION
            .byte FALLEN::DURATION
        DURATION_END:

        DURATION_COUNT = DURATION_END - DURATION

        .assert \
            DURATION_COUNT = FALLINGSTAR::STATE::COUNT, \
            error,                                      \
            "Falling star animation duration table size mismatch"

        ; @brief Number of frames per animation indexed by FALLINGSTAR::STATE
        FRAMES:
            .byte FALLING::COUNT
            .byte FALLEN::COUNT
        FRAMES_END:

        FRAMES_COUNT = FRAMES_END - FRAMES

        .assert \
            FRAMES_COUNT = FALLINGSTAR::STATE::COUNT, \
            error,                                    \
            "Falling star animation frame-count table size mismatch"

        ; @brief Animation next states indexed by FALLINGSTAR::STATE
        NEXT_STATE:
            .byte FALLING::NEXT_STATE
            .byte FALLEN::NEXT_STATE
        NEXT_STATE_END:

        NEXT_STATE_COUNT = NEXT_STATE_END - NEXT_STATE

        .assert \
            NEXT_STATE_COUNT = FALLINGSTAR::STATE::COUNT, \
            error,                                        \
            "Falling star animation next-state table size mismatch"
    .endscope
.endscope

.segment "ZEROPAGE"

; @brief Current animation frame sequence pointer
_fallingstar_animation_ptr:
    .res 2
; @brief Current falling star frame sprite table pointer
_fallingstar_frame_ptr:
    .res 2
; @brief Current collision map page pointer
_fallingstar_map_ptr:
    .res 2

.segment "BSS"

; @brief Allocated falling star data slot
_fallingstar_data_id:
    .res 1
; @brief Current pool index
_fallingstar_pool_index:
    .res 1
; @brief Source pool index for spawn
_fallingstar_spawn_source:
    .res 1
; @brief Prepared spawn X high byte
_fallingstar_spawn_worldx_hi:
    .res 1
; @brief Prepared spawn X low byte
_fallingstar_spawn_worldx_lo:
    .res 1
; @brief Prepared spawn Y
_fallingstar_spawn_worldy_lo:
    .res 1
; @brief Current falling star sprite X position
_fallingstar_screenx:
    .res 1
; @brief Current falling star sprite Y position
_fallingstar_screeny:
    .res 1
; @brief Current falling star sprite index
_fallingstar_sprite:
    .res 1
; @brief Current falling star sprite attributes
_fallingstar_attrs:
    .res 1
; @brief Current draw loop sprite index
_fallingstar_draw_index:
    .res 1
; @brief Number of sprites in the current frame
_fallingstar_draw_count:
    .res 1
; @brief Current foot probe X high byte
_fallingstar_probe_x_hi:
    .res 1
; @brief Current foot probe X low byte
_fallingstar_probe_x_lo:
    .res 1
; @brief Current foot probe Y
_fallingstar_probe_y:
    .res 1
; @brief Predicted Y movement for the current frame
_fallingstar_probe_dy:
    .res 1
; @brief Current collision map cell index
_fallingstar_map_index:
    .res 1

; @brief Falling star data slot usage markers
_fallingstar_data_used:
    .res FALLINGSTAR::DATA::MAX
; @brief Falling star current state per data slot
_fallingstar_data_state:
    .res FALLINGSTAR::DATA::MAX
; @brief Falling star animation frame index per data slot
_fallingstar_data_frame:
    .res FALLINGSTAR::DATA::MAX
; @brief Falling star animation timer per data slot
_fallingstar_data_timer:
    .res FALLINGSTAR::DATA::MAX

.segment "CODE"

; @brief Initializes the falling star object-specific data pool
.proc _init
    push a, x

    lda #FALLINGSTAR::DATA::FREE
    ldx #0

    loop:
        sta _fallingstar_data_used, x

        inx
        cpx #FALLINGSTAR::DATA::MAX
        bne loop

    pull a, x

    rts
.endproc

; @brief Allocates one falling star object-specific data slot
;
; @param[out] X data slot index
; @param[out] C clear if allocated, set if no free slots are available
.proc _fallingstar_data_alloc
    ldx #0

    loop:
        lda _fallingstar_data_used, x
        cmp #FALLINGSTAR::DATA::FREE
        beq found

        inx
        cpx #FALLINGSTAR::DATA::MAX
        bne loop

    sec
    rts

    found:
        clc
        rts
.endproc

; @brief Starts the falling state for a data slot
;
; @param[in] Y the index of the falling star data slot
.proc _fallingstar_data_start_falling
    lda #FALLINGSTAR::DATA::USED
    sta _fallingstar_data_used, y

    lda #FALLINGSTAR::STATE::FALLING
    sta _fallingstar_data_state, y

    lda #0
    sta _fallingstar_data_frame, y

    lda #FALLINGSTAR::ANIMATION::FALLING::DURATION
    sta _fallingstar_data_timer, y

    rts
.endproc

; @brief Spawns one falling star at the source pool object coordinates
;
; @param[in] X source pool object index
.export nsk_fallingstar_spawn
.proc nsk_fallingstar_spawn
    push a, x, y

    stx _fallingstar_spawn_source

    lda nsk_pool_size
    cmp #_NSK_POOL_MAX
    beq done

    jsr _fallingstar_data_alloc
    bcs done

    stx _fallingstar_data_id

    ldx _fallingstar_spawn_source
    lda nsk_pool_worldx_hi, x
    sta _fallingstar_spawn_worldx_hi
    lda nsk_pool_worldx_lo, x
    sta _fallingstar_spawn_worldx_lo
    lda nsk_pool_worldy_lo, x
    sta _fallingstar_spawn_worldy_lo

    ldy _fallingstar_data_id
    jsr _fallingstar_data_start_falling

    nsk_pool_add \
        { #SPRITELIST::FALLINGSTAR        }, \
        { #FALLINGSTAR::FLAGS             }, \
        { _fallingstar_spawn_worldx_hi    }, \
        { _fallingstar_spawn_worldx_lo    }, \
        { _fallingstar_spawn_worldy_lo    }, \
        { #FALLINGSTAR::VECTOR::X_LO      }, \
        { #FALLINGSTAR::VECTOR::X_FRAC    }, \
        { #FALLINGSTAR::VECTOR::Y_LO      }, \
        { #FALLINGSTAR::VECTOR::Y_FRAC    }, \
        { #FALLINGSTAR::RESPAWN::TIMER    }, \
        { _fallingstar_data_id            }

    done:
        pull a, x, y

    rts
.endproc

; @brief Routine to handle out-of-bounds falling star behavior
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_fallingstar_oob
.proc nsk_fallingstar_oob
    lda nsk_pool_flags, x
    ora #POOL::FLAGS::DELETED
    sta nsk_pool_flags, x

    rts
.endproc

; @brief Routine to clean up falling star object-specific data
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_fallingstar_clean
.proc nsk_fallingstar_clean
    ldy nsk_pool_data_id, x

    lda #FALLINGSTAR::DATA::FREE
    sta _fallingstar_data_used, y

    rts
.endproc

; @brief Selects the collision map page for the current foot probe
.proc _fallingstar_map_page_select
    lda #<nsk_map_grid
    sta _fallingstar_map_ptr + 0
    lda #>nsk_map_grid
    sta _fallingstar_map_ptr + 1

    lda _fallingstar_probe_x_hi
    beq done

    lda _fallingstar_map_ptr + 0
    clc
    adc #<MAP::SCREEN::PAGE
    sta _fallingstar_map_ptr + 0
    lda _fallingstar_map_ptr + 1
    adc #>MAP::SCREEN::PAGE
    sta _fallingstar_map_ptr + 1

    done:
        rts
.endproc

; @brief Checks if the current foot probe touches a solid collision block
;
; @param[in] _fallingstar_probe_x_hi Foot probe X high byte
; @param[in] _fallingstar_probe_x_lo Foot probe X low byte
; @param[in] _fallingstar_probe_y    Foot probe Y position
; @param[out] A 0 if empty, 1 if solid
.proc _fallingstar_ground_probe
    lda _fallingstar_probe_x_hi
    cmp #MAP::SCREEN::PAGES
    bcs empty

    lda _fallingstar_probe_y
    cmp #NSK::SCREEN::HEIGHT
    bcs empty

    jsr _fallingstar_map_page_select

    lda _fallingstar_probe_y
    and #$f0
    sta _fallingstar_map_index

    lda _fallingstar_probe_x_lo
    lsr
    lsr
    lsr
    lsr
    clc
    adc _fallingstar_map_index
    tay

    lda (_fallingstar_map_ptr), y
    cmp #MAP::COLLISION::SOLID
    beq solid

    empty:
        lda #0
        rts

    solid:
        lda #1
        rts
.endproc

; @brief Sets the falling star Y position to the top of the current ground block
.proc _fallingstar_snap_ground
    ldx _fallingstar_pool_index

    lda _fallingstar_probe_y
    and #$f0
    sec
    sbc #FALLINGSTAR::COLLISION::HEIGHT
    sta nsk_pool_worldy_lo, x

    lda #0
    sta nsk_pool_worldy_frac, x

    rts
.endproc

; @brief Sets the current foot probe X position from the object X and offset
;
; @param[in] A Foot probe X offset
.proc _fallingstar_probe_x_set
    ldx _fallingstar_pool_index

    clc
    adc nsk_pool_worldx_lo, x
    sta _fallingstar_probe_x_lo

    lda nsk_pool_worldx_hi, x
    adc #0
    sta _fallingstar_probe_x_hi

    rts
.endproc

; @brief Starts the grounded fallen state for the current falling star
;
; @param[in] Y the index of the falling star data slot
.proc _fallingstar_state_fallen
    lda #FALLINGSTAR::STATE::FALLEN
    sta _fallingstar_data_state, y

    lda #0
    sta _fallingstar_data_frame, y

    lda #FALLINGSTAR::ANIMATION::FALLEN::DURATION
    sta _fallingstar_data_timer, y

    ldx _fallingstar_pool_index

    lda #0
    sta nsk_pool_vectorx_lo, x
    sta nsk_pool_vectorx_frac, x
    sta nsk_pool_vectory_lo, x
    sta nsk_pool_vectory_frac, x

    lda nsk_pool_flags, x
    and #($ff - POOL::FLAGS::GRAVITY - POOL::FLAGS::VECTORS)
    sta nsk_pool_flags, x

    lda nsk_pool_worldy_lo, x
    clc
    adc #NSK::SCREEN::SPRITES::MODE_8X8::SPRITEHEIGHT
    sta nsk_pool_worldy_lo, x

    rts
.endproc

; @brief Routine to check whether falling star touches ground
;
; @param[in] X the index of the object in the nsk_pool_*
; @param[out] nsk_pool_result 0 if empty, non-zero if solid
.export nsk_fallingstar_isonground
.proc nsk_fallingstar_isonground
    push a, x, y

    stx _fallingstar_pool_index

    lda #0
    sta nsk_pool_result

    ldy nsk_pool_data_id, x
    lda _fallingstar_data_state, y
    cmp #FALLINGSTAR::STATE::FALLING
    bne done

    lda nsk_pool_vectory_frac, x
    clc
    adc #POOL::GRAVITY::ACCEL_FRAC

    lda nsk_pool_vectory_lo, x
    adc #POOL::GRAVITY::ACCEL_LO
    sta _fallingstar_probe_dy

    lda nsk_pool_worldy_lo, x
    clc
    adc _fallingstar_probe_dy
    bcs done

    clc
    adc #FALLINGSTAR::COLLISION::FOOT_Y
    bcs done
    sta _fallingstar_probe_y

    lda #FALLINGSTAR::COLLISION::CENTER_X
    jsr _fallingstar_probe_x_set
    jsr _fallingstar_ground_probe
    beq done

    grounded:
        jsr _fallingstar_snap_ground

        ldx _fallingstar_pool_index
        ldy nsk_pool_data_id, x
        jsr _fallingstar_state_fallen

        lda #1
        sta nsk_pool_result

    done:
        pull a, x, y

    rts
.endproc

; @brief Marks the current falling star for removal
.proc _fallingstar_delete
    ldx _fallingstar_pool_index

    lda nsk_pool_flags, x
    ora #POOL::FLAGS::DELETED
    sta nsk_pool_flags, x

    rts
.endproc

; @brief Ticks falling star animation state
;
; @param[in] Y the index of the falling star data slot
.proc _fallingstar_animation_tick
    lda _fallingstar_data_timer, y
    beq advance

    sec
    sbc #1
    sta _fallingstar_data_timer, y
    jmp done

    advance:
        lda _fallingstar_data_state, y
        tax

        lda _fallingstar_data_frame, y
        clc
        adc #1
        cmp FALLINGSTAR::ANIMATION::FRAMES, x
        bcc set_frame

        lda FALLINGSTAR::ANIMATION::NEXT_STATE, x
        sta _fallingstar_data_state, y
        tax

        lda #0

    set_frame:
        sta _fallingstar_data_frame, y

        lda FALLINGSTAR::ANIMATION::DURATION, x
        sta _fallingstar_data_timer, y

    done:
    rts
.endproc

; @brief Ticks the current falling star WHIRL respawn timer
.proc _fallingstar_respawn_tick
    ldx _fallingstar_pool_index

    lda nsk_pool_timer_a, x
    beq respawn

    sec
    sbc #1
    sta nsk_pool_timer_a, x
    bne done

    respawn:
        jsr nsk_whirl_spawn
        jsr _fallingstar_delete

    done:
        rts
.endproc

; @brief Routine to tick falling star-specific behavior
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_fallingstar_tick
.proc nsk_fallingstar_tick
    push a, x, y

    stx _fallingstar_pool_index

    ldy nsk_pool_data_id, x
    jsr _fallingstar_animation_tick

    jsr _fallingstar_respawn_tick

    pull a, x, y

    rts
.endproc

; @brief Selects the current falling star frame sprite table
;
; @param[in] Y the index of the falling star data slot
.proc _fallingstar_frame_select
    lda _fallingstar_data_state, y
    asl
    tax

    lda FALLINGSTAR::ANIMATION::TABLE + 0, x
    sta _fallingstar_animation_ptr + 0
    lda FALLINGSTAR::ANIMATION::TABLE + 1, x
    sta _fallingstar_animation_ptr + 1

    lda _fallingstar_data_frame, y
    asl
    tay

    lda (_fallingstar_animation_ptr), y
    sta _fallingstar_frame_ptr + 0
    iny
    lda (_fallingstar_animation_ptr), y
    sta _fallingstar_frame_ptr + 1

    rts
.endproc

; @brief Selects the current falling star frame sprite count
;
; @param[in] Y the index of the falling star data slot
.proc _fallingstar_draw_count_select
    lda _fallingstar_data_state, y
    cmp #FALLINGSTAR::STATE::FALLEN
    beq fallen

    lda #FALLINGSTAR::COUNT
    jmp done

    fallen:
        lda #FALLINGSTAR::FALLEN_COUNT

    done:
        sta _fallingstar_draw_count
    rts
.endproc

; @brief Draws the selected falling star frame
.proc _fallingstar_frame_draw
    ldx _fallingstar_pool_index
    ldy #0

    loop:
        sty _fallingstar_draw_index

        lda (_fallingstar_frame_ptr), y
        sta _fallingstar_sprite

        ldx _fallingstar_pool_index
        ldy nsk_pool_data_id, x
        lda _fallingstar_data_frame, y
        beq attrs_0

        ldy _fallingstar_draw_index
        lda FALLINGSTAR::DRAW_ATTRS_1::TABLE, y
        jmp attrs_done

        attrs_0:
            ldy _fallingstar_draw_index
            lda FALLINGSTAR::DRAW_ATTRS::TABLE, y

        attrs_done:
        sta _fallingstar_attrs

        clc
        lda nsk_pool_screenx
        adc FALLINGSTAR::POSX::TABLE, y
        bcs next
        sta _fallingstar_screenx

        clc
        lda nsk_pool_worldy_lo, x
        adc FALLINGSTAR::POSY::TABLE, y
        sta _fallingstar_screeny

        nsk_sprite_draw \
            { _fallingstar_sprite  }, \
            { _fallingstar_attrs   }, \
            { #FALLINGSTAR::PALETTE }, \
            { _fallingstar_screenx }, \
            { _fallingstar_screeny }

        next:
        ldy _fallingstar_draw_index
        iny
        cpy _fallingstar_draw_count
        bne loop

    rts
.endproc

; @brief Routine to draw the falling star
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_fallingstar_draw
.proc nsk_fallingstar_draw
    push a, x, y

    stx _fallingstar_pool_index

    ldy nsk_pool_data_id, x
    jsr _fallingstar_frame_select

    ldx _fallingstar_pool_index
    ldy nsk_pool_data_id, x
    jsr _fallingstar_draw_count_select

    jsr _fallingstar_frame_draw

    pull a, x, y

    rts
.endproc

.endif
