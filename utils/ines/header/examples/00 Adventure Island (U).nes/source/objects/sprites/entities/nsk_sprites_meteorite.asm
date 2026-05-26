; @file nsk_sprites_meteorite.asm
; @brief Source module for the meteorite sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_METEORITE_ASM__
::__NSK_SPRITES_METEORITE_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "nsk_sprites_meteorite.inc"

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

; @brief Meteorite settings
.scope METEORITE
    ; @brief Width of the meteorite metasprite in hardware sprites
    WIDTH = 2

    ; @brief Height of the falling/explosion metasprite in hardware sprites
    HEIGHT = 2

    ; @brief Number of hardware sprites per falling/explosion frame
    COUNT = WIDTH * HEIGHT

    ; @brief Width of the cooling metasprite in hardware sprites
    COOL_WIDTH = 2

    ; @brief Height of the cooling metasprite in hardware sprites
    COOL_HEIGHT = 1

    ; @brief Number of hardware sprites per cooling frame
    COOL_COUNT = COOL_WIDTH * COOL_HEIGHT

    ; @brief Meteorite sprite palette
    PALETTE = %01

    ; @brief Object flags
    FLAGS = POOL::FLAGS::VECTORS

    ; @brief Meteorite start Y
    STARTY = 0

    ; @brief Base meteorite sprite attributes
    ATTRS = \
        NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::NO        | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO

    ; @brief Horizontally flipped meteorite sprite attributes
    ATTRS_FLIPPED = \
        NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::YES       | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO

    ; @brief Meteorite frame X offsets
    .scope POSX
        TABLE:
            .byte 0,8
            .byte 0,8
        END:

        SIZE = END - TABLE

        .assert SIZE = METEORITE::COUNT, error, "Meteorite X offset table size mismatch"
    .endscope

    ; @brief Meteorite frame Y offsets
    .scope POSY
        TABLE:
            .byte 0,0
            .byte 8,8
        END:

        SIZE = END - TABLE

        .assert SIZE = METEORITE::COUNT, error, "Meteorite Y offset table size mismatch"
    .endscope

    ; @brief Meteorite animation states
    .scope STATE
        FALL   = 0 ; @< Flying/falling animation
        FALLEN = 1 ; @< Surface hit animation
        COOL   = 2 ; @< Cooling fade animation

        ; @brief Number of meteorite animation states
        COUNT = 3
    .endscope

    ; @brief Meteorite direction states
    .scope DIRECTION
        LEFT  = 0 ; @< Falling right-to-left
        RIGHT = 1 ; @< Falling left-to-right
    .endscope

    ; @brief Meteorite collision probes
    .scope COLLISION
        ; @brief Left foot probe offset
        LEFT_X = 0

        ; @brief Right foot probe offset
        RIGHT_X = METEORITE::WIDTH * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEWIDTH - 1

        ; @brief Foot probe Y offset
        FOOT_Y = METEORITE::HEIGHT * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEHEIGHT

        ; @brief Collision box height
        HEIGHT = FOOT_Y
    .endscope

    ; @brief Meteorite movement vectors
    .scope VECTOR
        ; @brief Left movement low byte
        LEFT_X_LO = $ff

        ; @brief Left movement fractional byte
        LEFT_X_FRAC = $b0

        ; @brief Right movement low byte
        RIGHT_X_LO = $00

        ; @brief Right movement fractional byte
        RIGHT_X_FRAC = $b0

        ; @brief Down movement low byte
        Y_LO = $01

        ; @brief Down movement fractional byte
        Y_FRAC = $40
    .endscope

    ; @brief Meteorite object-specific data storage
    .scope DATA
        ; @brief Maximum number of meteorite object-specific data slots
        MAX = 2

        ; @brief Free data slot marker
        FREE = $ff

        ; @brief Used data slot marker
        USED = $00
    .endscope

    ; @brief Meteorite animation frames
    .scope FRAME
        ; @brief First falling animation frame
        .scope FALL_0
            TABLE:
                .byte $26,$27
                .byte $36,$37
            END:

            SIZE = END - TABLE

            .assert SIZE = METEORITE::COUNT, error, "Meteorite fall 0 frame size mismatch"
        .endscope

        ; @brief Second falling animation frame
        .scope FALL_1
            TABLE:
                .byte $28,$29
                .byte $38,$39
            END:

            SIZE = END - TABLE

            .assert SIZE = METEORITE::COUNT, error, "Meteorite fall 1 frame size mismatch"
        .endscope

        ; @brief First surface hit animation frame
        .scope FALLEN_0
            TABLE:
                .byte $2a,$2b
                .byte $3a,$3b
            END:

            SIZE = END - TABLE

            .assert SIZE = METEORITE::COUNT, error, "Meteorite fallen 0 frame size mismatch"
        .endscope

        ; @brief Second surface hit animation frame
        .scope FALLEN_1
            TABLE:
                .byte $2c,$2d
                .byte $3c,$3d
            END:

            SIZE = END - TABLE

            .assert SIZE = METEORITE::COUNT, error, "Meteorite fallen 1 frame size mismatch"
        .endscope

        ; @brief First cooling animation frame
        .scope COOL_0
            TABLE:
                .byte $3e,$3f
            END:

            SIZE = END - TABLE

            .assert SIZE = METEORITE::COOL_COUNT, error, "Meteorite cool 0 frame size mismatch"
        .endscope

        ; @brief Second cooling animation frame
        .scope COOL_1
            TABLE:
                .byte $2e,$2f
            END:

            SIZE = END - TABLE

            .assert SIZE = METEORITE::COOL_COUNT, error, "Meteorite cool 1 frame size mismatch"
        .endscope
    .endscope

    ; @brief Meteorite animation frame sequences
    .scope ANIMATION
        ; @brief Falling animation frames
        .scope FALL
            ; @brief Number of game frames per animation frame
            DURATION = 4

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = METEORITE::STATE::FALL

            TABLE:
                .addr METEORITE::FRAME::FALL_0::TABLE
                .addr METEORITE::FRAME::FALL_1::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Surface hit animation frames
        .scope FALLEN
            ; @brief Number of game frames per animation frame
            DURATION = 8

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = METEORITE::STATE::COOL

            TABLE:
                .addr METEORITE::FRAME::FALLEN_0::TABLE
                .addr METEORITE::FRAME::FALLEN_1::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Cooling animation frames
        .scope COOL
            ; @brief Number of game frames per animation frame
            DURATION = 16

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = METEORITE::STATE::COOL

            TABLE:
                .addr METEORITE::FRAME::COOL_0::TABLE
                .addr METEORITE::FRAME::COOL_1::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Animation frame tables indexed by METEORITE::STATE
        TABLE:
            .addr FALL::TABLE
            .addr FALLEN::TABLE
            .addr COOL::TABLE
        END:

        COUNT = (END - TABLE) / 2

        .assert COUNT = METEORITE::STATE::COUNT, error, "Meteorite animation table size mismatch"

        ; @brief Animation frame durations indexed by METEORITE::STATE
        DURATION:
            .byte FALL::DURATION
            .byte FALLEN::DURATION
            .byte COOL::DURATION
        DURATION_END:

        DURATION_COUNT = DURATION_END - DURATION

        .assert \
            DURATION_COUNT = METEORITE::STATE::COUNT, \
            error,                                    \
            "Meteorite animation duration table size mismatch"

        ; @brief Number of frames per animation indexed by METEORITE::STATE
        FRAMES:
            .byte FALL::COUNT
            .byte FALLEN::COUNT
            .byte COOL::COUNT
        FRAMES_END:

        FRAMES_COUNT = FRAMES_END - FRAMES

        .assert \
            FRAMES_COUNT = METEORITE::STATE::COUNT, \
            error,                                  \
            "Meteorite animation frame-count table size mismatch"

        ; @brief Animation next states indexed by METEORITE::STATE
        NEXT_STATE:
            .byte FALL::NEXT_STATE
            .byte FALLEN::NEXT_STATE
            .byte COOL::NEXT_STATE
        NEXT_STATE_END:

        NEXT_STATE_COUNT = NEXT_STATE_END - NEXT_STATE

        .assert \
            NEXT_STATE_COUNT = METEORITE::STATE::COUNT, \
            error,                                      \
            "Meteorite animation next-state table size mismatch"
    .endscope
.endscope

.segment "ZEROPAGE"

; @brief Current animation frame sequence pointer
_meteorite_animation_ptr:
    .res 2
; @brief Current meteorite frame sprite table pointer
_meteorite_frame_ptr:
    .res 2
; @brief Current collision map page pointer
_meteorite_map_ptr:
    .res 2

.segment "BSS"

; @brief Current meteorite sprite X position
_meteorite_screenx:
    .res 1
; @brief Current meteorite sprite Y position
_meteorite_screeny:
    .res 1
; @brief Allocated meteorite data slot
_meteorite_data_id:
    .res 1
; @brief Current pool index
_meteorite_pool_index:
    .res 1
; @brief Current meteorite sprite index
_meteorite_sprite:
    .res 1
; @brief Current meteorite sprite attributes
_meteorite_attrs:
    .res 1
; @brief Current draw loop sprite index
_meteorite_draw_index:
    .res 1
; @brief Current draw loop sprite source index
_meteorite_source_index:
    .res 1
; @brief Current draw loop sprite column
_meteorite_draw_col:
    .res 1
; @brief Current draw loop sprite row offset
_meteorite_draw_row:
    .res 1
; @brief Number of sprites in the current frame
_meteorite_draw_count:
    .res 1
; @brief Current foot probe X high byte
_meteorite_probe_x_hi:
    .res 1
; @brief Current foot probe X low byte
_meteorite_probe_x_lo:
    .res 1
; @brief Current foot probe Y
_meteorite_probe_y:
    .res 1
; @brief Current collision map cell index
_meteorite_map_index:
    .res 1
; @brief Prepared spawn X high byte
_meteorite_spawn_worldx_hi:
    .res 1
; @brief Prepared spawn X low byte
_meteorite_spawn_worldx_lo:
    .res 1
; @brief Prepared spawn direction
_meteorite_spawn_direction:
    .res 1
; @brief Prepared spawn X vector low byte
_meteorite_spawn_vectorx_lo:
    .res 1
; @brief Prepared spawn X vector fractional byte
_meteorite_spawn_vectorx_frac:
    .res 1

; @brief Meteorite data slot usage markers
_meteorite_data_used:
    .res METEORITE::DATA::MAX
; @brief Meteorite current state per data slot
_meteorite_data_state:
    .res METEORITE::DATA::MAX
; @brief Meteorite direction per data slot
_meteorite_data_direction:
    .res METEORITE::DATA::MAX
; @brief Meteorite animation frame index per data slot
_meteorite_data_frame:
    .res METEORITE::DATA::MAX
; @brief Meteorite animation timer per data slot
_meteorite_data_timer:
    .res METEORITE::DATA::MAX

.segment "CODE"

; @brief Initializes the meteorite object-specific data pool
.proc _init
    push a, x

    lda #METEORITE::DATA::FREE
    ldx #0

    loop:
        sta _meteorite_data_used, x

        inx
        cpx #METEORITE::DATA::MAX
        bne loop

    pull a, x

    rts
.endproc

; @brief Allocates one meteorite object-specific data slot
;
; @param[out] X data slot index
; @param[out] C clear if allocated, set if no free slots are available
.proc _meteorite_data_alloc
    ldx #0

    loop:
        lda _meteorite_data_used, x
        cmp #METEORITE::DATA::FREE
        beq found

        inx
        cpx #METEORITE::DATA::MAX
        bne loop

    sec
    rts

    found:
        clc
        rts
.endproc

; @brief Prepares random spawn parameters
.proc _meteorite_spawn_prepare
    jsr nsk_util_rand8
    and #1
    sta _meteorite_spawn_worldx_hi

    jsr nsk_util_rand8
    sta _meteorite_spawn_worldx_lo

    jsr nsk_util_rand8
    and #1
    sta _meteorite_spawn_direction
    beq left

    right:
        lda #METEORITE::VECTOR::RIGHT_X_LO
        sta _meteorite_spawn_vectorx_lo
        lda #METEORITE::VECTOR::RIGHT_X_FRAC
        sta _meteorite_spawn_vectorx_frac
        rts

    left:
        lda #METEORITE::VECTOR::LEFT_X_LO
        sta _meteorite_spawn_vectorx_lo
        lda #METEORITE::VECTOR::LEFT_X_FRAC
        sta _meteorite_spawn_vectorx_frac
        rts
.endproc

; @brief Starts the falling state for a data slot
;
; @param[in] Y the index of the meteorite data slot
.proc _meteorite_data_start_fall
    lda #METEORITE::DATA::USED
    sta _meteorite_data_used, y

    lda #METEORITE::STATE::FALL
    sta _meteorite_data_state, y

    lda _meteorite_spawn_direction
    sta _meteorite_data_direction, y

    lda #0
    sta _meteorite_data_frame, y

    lda #METEORITE::ANIMATION::FALL::DURATION
    sta _meteorite_data_timer, y

    rts
.endproc

; @brief Spawns one meteorite in the object pool
.proc _meteorite_spawn
    push a, x, y

    lda nsk_pool_size
    cmp #_NSK_POOL_MAX
    beq done

    jsr _meteorite_data_alloc
    bcs done

    stx _meteorite_data_id

    jsr _meteorite_spawn_prepare

    ldy _meteorite_data_id
    jsr _meteorite_data_start_fall

    nsk_pool_add \
        { #SPRITELIST::METEORITE           }, \
        { #METEORITE::FLAGS                }, \
        { _meteorite_spawn_worldx_hi       }, \
        { _meteorite_spawn_worldx_lo       }, \
        { #METEORITE::STARTY               }, \
        { _meteorite_spawn_vectorx_lo      }, \
        { _meteorite_spawn_vectorx_frac    }, \
        { #METEORITE::VECTOR::Y_LO         }, \
        { #METEORITE::VECTOR::Y_FRAC       }, \
        { #0                               }, \
        { _meteorite_data_id               }

    done:
        pull a, x, y

    rts
.endproc

; @brief Resets the current pool slot as a fresh falling meteorite
.proc _meteorite_reset_current
    jsr _meteorite_spawn_prepare

    ldx _meteorite_pool_index

    lda #METEORITE::FLAGS
    sta nsk_pool_flags, x

    lda _meteorite_spawn_worldx_hi
    sta nsk_pool_worldx_hi, x
    lda _meteorite_spawn_worldx_lo
    sta nsk_pool_worldx_lo, x
    lda #0
    sta nsk_pool_worldx_frac, x

    lda #METEORITE::STARTY
    sta nsk_pool_worldy_lo, x
    lda #0
    sta nsk_pool_worldy_frac, x

    lda _meteorite_spawn_vectorx_lo
    sta nsk_pool_vectorx_lo, x
    lda _meteorite_spawn_vectorx_frac
    sta nsk_pool_vectorx_frac, x

    lda #METEORITE::VECTOR::Y_LO
    sta nsk_pool_vectory_lo, x
    lda #METEORITE::VECTOR::Y_FRAC
    sta nsk_pool_vectory_frac, x

    lda #0
    sta nsk_pool_timer_a, x

    ldy nsk_pool_data_id, x
    jsr _meteorite_data_start_fall

    rts
.endproc

; @brief Inits the meteorites in the object pool
.export nsk_meteorite_init
.proc nsk_meteorite_init
    push x

    ldx #0

    loop:
        jsr _meteorite_spawn

        inx
        cpx #METEORITE::DATA::MAX
        bne loop

    pull x

    rts
.endproc

; @brief Routine to handle out-of-bounds meteorite behavior
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_meteorite_oob
.proc nsk_meteorite_oob
    push a, x, y

    stx _meteorite_pool_index
    jsr _meteorite_reset_current

    pull a, x, y

    rts
.endproc

; @brief Routine to clean up meteorite object-specific data
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_meteorite_clean
.proc nsk_meteorite_clean
    push a, y

    ldy nsk_pool_data_id, x

    lda #METEORITE::DATA::FREE
    sta _meteorite_data_used, y

    jsr _meteorite_spawn

    pull a, y

    rts
.endproc

; @brief Selects the collision map page for the current foot probe
.proc _meteorite_map_page_select
    lda #<nsk_map_grid
    sta _meteorite_map_ptr + 0
    lda #>nsk_map_grid
    sta _meteorite_map_ptr + 1

    lda _meteorite_probe_x_hi
    beq done

    lda _meteorite_map_ptr + 0
    clc
    adc #<MAP::SCREEN::PAGE
    sta _meteorite_map_ptr + 0
    lda _meteorite_map_ptr + 1
    adc #>MAP::SCREEN::PAGE
    sta _meteorite_map_ptr + 1

    done:
        rts
.endproc

; @brief Checks if the current foot probe touches a solid collision block
;
; @param[in] _meteorite_probe_x_hi Foot probe X high byte
; @param[in] _meteorite_probe_x_lo Foot probe X low byte
; @param[in] _meteorite_probe_y    Foot probe Y position
; @param[out] A 0 if empty, 1 if solid
.proc _meteorite_ground_probe
    lda _meteorite_probe_x_hi
    cmp #MAP::SCREEN::PAGES
    bcs empty

    lda _meteorite_probe_y
    cmp #NSK::SCREEN::HEIGHT
    bcs empty

    jsr _meteorite_map_page_select

    lda _meteorite_probe_y
    and #$f0
    sta _meteorite_map_index

    lda _meteorite_probe_x_lo
    lsr
    lsr
    lsr
    lsr
    clc
    adc _meteorite_map_index
    tay

    lda (_meteorite_map_ptr), y
    cmp #MAP::COLLISION::SOLID
    beq solid

    empty:
        lda #0
        rts

    solid:
        lda #1
        rts
.endproc

; @brief Sets the meteorite Y position to the top of the current ground block
.proc _meteorite_snap_ground
    ldx _meteorite_pool_index

    lda _meteorite_probe_y
    and #$f0
    sec
    sbc #METEORITE::COLLISION::HEIGHT
    sta nsk_pool_worldy_lo, x

    lda #0
    sta nsk_pool_worldy_frac, x

    rts
.endproc

; @brief Sets the current foot probe X position from the object X and offset
;
; @param[in] A Foot probe X offset
.proc _meteorite_probe_x_set
    ldx _meteorite_pool_index

    clc
    adc nsk_pool_worldx_lo, x
    sta _meteorite_probe_x_lo

    lda nsk_pool_worldx_hi, x
    adc #0
    sta _meteorite_probe_x_hi

    rts
.endproc

; @brief Checks if the meteorite touches ground
;
; @param[out] nsk_pool_result 0 if empty, non-zero if solid
.proc _meteorite_ground_check
    push a, x, y

    lda #0
    sta nsk_pool_result

    ldx _meteorite_pool_index

    lda nsk_pool_worldy_lo, x
    clc
    adc #METEORITE::COLLISION::FOOT_Y
    bcs done
    sta _meteorite_probe_y

    lda #METEORITE::COLLISION::LEFT_X
    jsr _meteorite_probe_x_set
    jsr _meteorite_ground_probe
    bne grounded

    lda #METEORITE::COLLISION::RIGHT_X
    jsr _meteorite_probe_x_set
    jsr _meteorite_ground_probe
    beq done

    grounded:
        jsr _meteorite_snap_ground

        lda #1
        sta nsk_pool_result

    done:
        pull a, x, y

    rts
.endproc

; @brief Starts the surface hit state for the current meteorite
;
; @param[in] Y the index of the meteorite data slot
.proc _meteorite_state_fallen
    lda #METEORITE::STATE::FALLEN
    sta _meteorite_data_state, y

    lda #0
    sta _meteorite_data_frame, y

    lda #METEORITE::ANIMATION::FALLEN::DURATION
    sta _meteorite_data_timer, y

    ldx _meteorite_pool_index

    lda #0
    sta nsk_pool_vectorx_lo, x
    sta nsk_pool_vectorx_frac, x
    sta nsk_pool_vectory_lo, x
    sta nsk_pool_vectory_frac, x

    rts
.endproc

; @brief Marks the current meteorite for removal
.proc _meteorite_delete
    ldx _meteorite_pool_index

    lda nsk_pool_flags, x
    ora #POOL::FLAGS::DELETED
    sta nsk_pool_flags, x

    rts
.endproc

; @brief Ticks meteorite animation state
;
; @param[in] Y the index of the meteorite data slot
.proc _meteorite_animation_tick
    lda _meteorite_data_timer, y
    beq advance

    sec
    sbc #1
    sta _meteorite_data_timer, y
    jmp done

    advance:
        lda _meteorite_data_state, y
        tax

        lda _meteorite_data_frame, y
        clc
        adc #1
        cmp METEORITE::ANIMATION::FRAMES, x
        bcc set_frame

        lda _meteorite_data_state, y
        cmp #METEORITE::STATE::COOL
        beq delete

        lda METEORITE::ANIMATION::NEXT_STATE, x
        sta _meteorite_data_state, y
        tax

        lda #0

    set_frame:
        sta _meteorite_data_frame, y

        lda METEORITE::ANIMATION::DURATION, x
        sta _meteorite_data_timer, y
        jmp done

    delete:
        jsr _meteorite_delete

    done:
        rts
.endproc

; @brief Routine to tick meteorite-specific behavior
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_meteorite_tick
.proc nsk_meteorite_tick
    push a, x, y

    stx _meteorite_pool_index

    ldy nsk_pool_data_id, x

    lda _meteorite_data_state, y
    cmp #METEORITE::STATE::FALL
    bne animate

    jsr _meteorite_ground_check
    lda nsk_pool_result
    beq animate

    ldy nsk_pool_data_id, x
    jsr _meteorite_state_fallen
    jmp done

    animate:
        ldy nsk_pool_data_id, x
        jsr _meteorite_animation_tick

    done:
        pull a, x, y

    rts
.endproc

; @brief Selects the current meteorite frame sprite table
;
; @param[in] Y the index of the meteorite data slot
.proc _meteorite_frame_select
    lda _meteorite_data_state, y
    asl
    tax

    lda METEORITE::ANIMATION::TABLE + 0, x
    sta _meteorite_animation_ptr + 0
    lda METEORITE::ANIMATION::TABLE + 1, x
    sta _meteorite_animation_ptr + 1

    lda _meteorite_data_frame, y
    asl
    tay

    lda (_meteorite_animation_ptr), y
    sta _meteorite_frame_ptr + 0
    iny
    lda (_meteorite_animation_ptr), y
    sta _meteorite_frame_ptr + 1

    rts
.endproc

; @brief Selects the current meteorite sprite attributes
;
; @param[in] Y the index of the meteorite data slot
.proc _meteorite_attrs_select
    lda _meteorite_data_direction, y
    cmp #METEORITE::DIRECTION::RIGHT
    beq flipped

    lda #METEORITE::ATTRS
    jmp done

    flipped:
        lda #METEORITE::ATTRS_FLIPPED

    done:
        sta _meteorite_attrs
    rts
.endproc

; @brief Selects the current meteorite frame sprite count
;
; @param[in] Y the index of the meteorite data slot
.proc _meteorite_draw_count_select
    lda _meteorite_data_state, y
    cmp #METEORITE::STATE::COOL
    beq cool

    lda #METEORITE::COUNT
    jmp done

    cool:
        lda #METEORITE::COOL_COUNT

    done:
        sta _meteorite_draw_count
    rts
.endproc

; @brief Selects the source sprite index for the current draw index
.proc _meteorite_source_index_select
    lda _meteorite_draw_index
    sta _meteorite_source_index

    lda _meteorite_attrs
    cmp #METEORITE::ATTRS_FLIPPED
    bne done

    lda _meteorite_draw_index
    sta _meteorite_draw_col
    lda #0
    sta _meteorite_draw_row

    column:
        lda _meteorite_draw_col
        cmp #METEORITE::WIDTH
        bcc column_done

        sec
        sbc #METEORITE::WIDTH
        sta _meteorite_draw_col

        lda _meteorite_draw_row
        clc
        adc #METEORITE::WIDTH
        sta _meteorite_draw_row
        jmp column

    column_done:
        lda #METEORITE::WIDTH - 1
        sec
        sbc _meteorite_draw_col
        clc
        adc _meteorite_draw_row
        sta _meteorite_source_index

    done:
        rts
.endproc

; @brief Draws the selected meteorite frame
.proc _meteorite_frame_draw
    ldx _meteorite_pool_index
    ldy #0

    loop:
        sty _meteorite_draw_index

        jsr _meteorite_source_index_select
        ldy _meteorite_source_index
        lda (_meteorite_frame_ptr), y
        sta _meteorite_sprite
        ldy _meteorite_draw_index

        clc
        lda nsk_pool_screenx
        adc METEORITE::POSX::TABLE, y
        sta _meteorite_screenx

        clc
        lda nsk_pool_worldy_lo, x
        adc METEORITE::POSY::TABLE, y
        sta _meteorite_screeny

        nsk_sprite_draw \
            { _meteorite_sprite  }, \
            { _meteorite_attrs   }, \
            { #METEORITE::PALETTE }, \
            { _meteorite_screenx }, \
            { _meteorite_screeny }

        iny
        cpy _meteorite_draw_count
        bne loop

    rts
.endproc

; @brief Routine to draw the meteorite
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_meteorite_draw
.proc nsk_meteorite_draw
    push a, x, y

    stx _meteorite_pool_index

    ldy nsk_pool_data_id, x

    jsr _meteorite_frame_select

    ldx _meteorite_pool_index
    ldy nsk_pool_data_id, x
    jsr _meteorite_attrs_select

    ldx _meteorite_pool_index
    ldy nsk_pool_data_id, x
    jsr _meteorite_draw_count_select

    jsr _meteorite_frame_draw

    pull a, x, y

    rts
.endproc

.endif
