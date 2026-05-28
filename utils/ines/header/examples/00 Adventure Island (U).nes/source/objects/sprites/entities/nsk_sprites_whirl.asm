; @file nsk_sprites_whirl.asm
; @brief Source module for the whirl sprites data and functions
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_SPRITES_WHIRL_ASM__
::__NSK_SPRITES_WHIRL_ASM__ = 1

.linecont +

.include "nsk_common_meta.inc"

.include "nsk_sprites_whirl.inc"

.include "../draw/nsk_sprites_draw.inc"
.include "../pool/nsk_pool_add.inc"
.include "../pool/nsk_pool_attrs.inc"
.include "../pool/nsk_pool_settings.inc"
.include "../pool/nsk_pool_vars.inc"
.include "../nsk_sprites_list.inc"
.include "nsk_sprites_fallingstar.inc"
.include "nsk_sprites_starbit.inc"
.include "../../../utils/nsk_util_rand8.inc"

nsk_constructor _init

.segment "RODATA"

; @brief Whirl settings
.scope WHIRL
    ; @brief Width of the whirl metasprite in hardware sprites
    WIDTH = 2

    ; @brief Height of the whirl metasprite in hardware sprites
    HEIGHT = 2

    ; @brief Number of hardware sprites per whirl frame
    COUNT = WIDTH * HEIGHT

    ; @brief Whirl sprite palette
    PALETTE = %00

    ; @brief Object flags
    FLAGS = POOL::FLAGS::COLLISION

    ; @brief Whirl start Y
    STARTY = 130

    ; @brief Whirl sprite attributes
    ATTRS = \
        NSK::PPU::SPRITES::ATTRIBUTE::PRIORITY::FOREGROUND | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::HOR::NO        | \
        NSK::PPU::SPRITES::ATTRIBUTE::FLIP::VER::NO

    ; @brief Whirl frame X offsets
    .scope POSX
        TABLE:
            .byte 0,8
            .byte 0,8
        END:

        SIZE = END - TABLE

        .assert SIZE = WHIRL::COUNT, error, "Whirl X offset table size mismatch"
    .endscope

    ; @brief Whirl frame Y offsets
    .scope POSY
        TABLE:
            .byte 0,0
            .byte 8,8
        END:

        SIZE = END - TABLE

        .assert SIZE = WHIRL::COUNT, error, "Whirl Y offset table size mismatch"
    .endscope

    ; @brief Whirl animation states
    .scope STATE
        IDLE = 0 ; @< Idle animation
        KILL = 1 ; @< Kill animation

        ; @brief Number of whirl animation states
        COUNT = 2
    .endscope

    ; @brief Whirl spawn settings
    .scope SPAWN
        ; @brief Left spawn edge high byte
        LEFT_HI = 0

        ; @brief Left spawn edge low byte
        LEFT_LO = 224

        ; @brief Right spawn edge high byte
        RIGHT_HI = 1

        ; @brief Right spawn edge low byte
        RIGHT_LO = 85

        ; @brief Number of low-page spawn positions
        LEFT_COUNT = 256 - LEFT_LO

        ; @brief Number of high-page spawn positions
        RIGHT_COUNT = RIGHT_LO + 1
    .endscope

    ; @brief Whirl object-specific data storage
    .scope DATA
        ; @brief Maximum number of whirl object-specific data slots
        MAX = 1

        ; @brief Free data slot marker
        FREE = $ff

        ; @brief Used data slot marker
        USED = $00
    .endscope

    ; @brief Whirl animation frames
    .scope FRAME
        ; @brief First idle animation frame
        .scope IDLE_0
            TABLE:
                .byte $98,$99
                .byte $a8,$a9
            END:

            SIZE = END - TABLE

            .assert SIZE = WHIRL::COUNT, error, "Whirl idle 0 frame size mismatch"
        .endscope

        ; @brief Second idle animation frame
        .scope IDLE_1
            TABLE:
                .byte $9a,$9b
                .byte $aa,$ab
            END:

            SIZE = END - TABLE

            .assert SIZE = WHIRL::COUNT, error, "Whirl idle 1 frame size mismatch"
        .endscope

        ; @brief Third idle animation frame
        .scope IDLE_2
            TABLE:
                .byte $9c,$9d
                .byte $ac,$ad
            END:

            SIZE = END - TABLE

            .assert SIZE = WHIRL::COUNT, error, "Whirl idle 2 frame size mismatch"
        .endscope

        ; @brief Kill animation frame
        .scope KILL_0
            TABLE:
                .byte $9e,$9f
                .byte $ae,$af
            END:

            SIZE = END - TABLE

            .assert SIZE = WHIRL::COUNT, error, "Whirl kill frame size mismatch"
        .endscope
    .endscope

    ; @brief Whirl animation frame sequences
    .scope ANIMATION
        ; @brief Idle animation frames
        .scope IDLE
            ; @brief Number of game frames per animation frame
            DURATION = 12

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = WHIRL::STATE::IDLE

            TABLE:
                .addr WHIRL::FRAME::IDLE_0::TABLE
                .addr WHIRL::FRAME::IDLE_1::TABLE
                .addr WHIRL::FRAME::IDLE_2::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Kill animation frames
        .scope KILL
            ; @brief Number of game frames per animation frame
            DURATION = 8

            ; @brief State to switch to when the animation reaches its end
            NEXT_STATE = WHIRL::STATE::KILL

            TABLE:
                .addr WHIRL::FRAME::KILL_0::TABLE
            END:

            COUNT = (END - TABLE) / 2
        .endscope

        ; @brief Animation frame tables indexed by WHIRL::STATE
        TABLE:
            .addr IDLE::TABLE
            .addr KILL::TABLE
        END:

        COUNT = (END - TABLE) / 2

        .assert COUNT = WHIRL::STATE::COUNT, error, "Whirl animation table size mismatch"

        ; @brief Animation frame durations indexed by WHIRL::STATE
        DURATION:
            .byte IDLE::DURATION
            .byte KILL::DURATION
        DURATION_END:

        DURATION_COUNT = DURATION_END - DURATION

        .assert \
            DURATION_COUNT = WHIRL::STATE::COUNT, \
            error,                                \
            "Whirl animation duration table size mismatch"

        ; @brief Number of frames per animation indexed by WHIRL::STATE
        FRAMES:
            .byte IDLE::COUNT
            .byte KILL::COUNT
        FRAMES_END:

        FRAMES_COUNT = FRAMES_END - FRAMES

        .assert \
            FRAMES_COUNT = WHIRL::STATE::COUNT, \
            error,                              \
            "Whirl animation frame-count table size mismatch"

        ; @brief Animation next states indexed by WHIRL::STATE
        NEXT_STATE:
            .byte IDLE::NEXT_STATE
            .byte KILL::NEXT_STATE
        NEXT_STATE_END:

        NEXT_STATE_COUNT = NEXT_STATE_END - NEXT_STATE

        .assert \
            NEXT_STATE_COUNT = WHIRL::STATE::COUNT, \
            error,                                  \
            "Whirl animation next-state table size mismatch"
    .endscope
.endscope

.segment "ZEROPAGE"

; @brief Current animation frame sequence pointer
_whirl_animation_ptr:
    .res 2
; @brief Current whirl frame sprite table pointer
_whirl_frame_ptr:
    .res 2

.segment "BSS"

; @brief Allocated whirl data slot
_whirl_data_id:
    .res 1
; @brief Current pool index
_whirl_pool_index:
    .res 1
; @brief Prepared spawn X high byte
_whirl_spawn_worldx_hi:
    .res 1
; @brief Prepared spawn X low byte
_whirl_spawn_worldx_lo:
    .res 1
; @brief Current whirl sprite X position
_whirl_screenx:
    .res 1
; @brief Current whirl sprite Y position
_whirl_screeny:
    .res 1
; @brief Current whirl sprite index
_whirl_sprite:
    .res 1
; @brief Current pool scan index
_whirl_scan_index:
    .res 1
; @brief Best pool index found by the nearest query
_whirl_best_index:
    .res 1
; @brief Best distance high byte found by the nearest query
_whirl_best_distance_hi:
    .res 1
; @brief Best distance low byte found by the nearest query
_whirl_best_distance_lo:
    .res 1
; @brief Candidate distance high byte for the nearest query
_whirl_distance_hi:
    .res 1
; @brief Candidate distance low byte for the nearest query
_whirl_distance_lo:
    .res 1

; @brief Query X high byte for nsk_whirl_nearest_find
.export nsk_whirl_query_x_hi
nsk_whirl_query_x_hi:
    .res 1
; @brief Query X low byte for nsk_whirl_nearest_find
.export nsk_whirl_query_x_lo
nsk_whirl_query_x_lo:
    .res 1
; @brief Query direction for nsk_whirl_nearest_find (LEFT=0, RIGHT=1)
.export nsk_whirl_query_direction
nsk_whirl_query_direction:
    .res 1

; @brief Whirl data slot usage markers
_whirl_data_used:
    .res WHIRL::DATA::MAX
; @brief Whirl current state per data slot
_whirl_data_state:
    .res WHIRL::DATA::MAX
; @brief Whirl animation frame index per data slot
_whirl_data_frame:
    .res WHIRL::DATA::MAX
; @brief Whirl animation timer per data slot
_whirl_data_timer:
    .res WHIRL::DATA::MAX

.segment "CODE"

; @brief Initializes the whirl object-specific data pool
.proc _init
    push a, x

    ldx #0

    loop:
        lda #WHIRL::DATA::FREE
        sta _whirl_data_used, x
        inx
        cpx #WHIRL::DATA::MAX
        bne loop

    pull a, x

    rts
.endproc

; @brief Allocates one whirl object-specific data slot
;
; @param[out] X data slot index
; @param[out] C clear if allocated, set if no free slots are available
.proc _whirl_data_alloc
    ldx #0

    loop:
        lda _whirl_data_used, x
        cmp #WHIRL::DATA::FREE
        beq found

        inx
        cpx #WHIRL::DATA::MAX
        bne loop

    sec
    rts

    found:
        clc
        rts
.endproc

; @brief Prepares random spawn X inside the patrol range
.proc _whirl_spawn_x_prepare
    jsr nsk_util_rand8
    and #1
    beq left_page

    right_page:
        lda #WHIRL::SPAWN::RIGHT_HI
        sta _whirl_spawn_worldx_hi

        retry_right:
            jsr nsk_util_rand8
            cmp #WHIRL::SPAWN::RIGHT_COUNT
            bcs retry_right

        sta _whirl_spawn_worldx_lo
        rts

    left_page:
        lda #WHIRL::SPAWN::LEFT_HI
        sta _whirl_spawn_worldx_hi

        retry_left:
            jsr nsk_util_rand8
            cmp #WHIRL::SPAWN::LEFT_COUNT
            bcs retry_left

        clc
        adc #WHIRL::SPAWN::LEFT_LO
        sta _whirl_spawn_worldx_lo

        rts
.endproc

; @brief Starts the idle state for a data slot
;
; @param[in] Y the index of the whirl data slot
.proc _whirl_data_start_idle
    lda #WHIRL::DATA::USED
    sta _whirl_data_used, y

    lda #WHIRL::STATE::IDLE
    sta _whirl_data_state, y

    lda #0
    sta _whirl_data_frame, y

    lda #WHIRL::ANIMATION::IDLE::DURATION
    sta _whirl_data_timer, y

    rts
.endproc

; @brief Spawns one whirl in the object pool
.proc _whirl_spawn
    push a, x, y

    lda nsk_pool_size
    cmp #_NSK_POOL_MAX
    beq done

    jsr _whirl_data_alloc
    bcs done

    stx _whirl_data_id

    jsr _whirl_spawn_x_prepare

    ldy _whirl_data_id
    jsr _whirl_data_start_idle

    nsk_pool_add \
        { #SPRITELIST::WHIRL        }, \
        { #WHIRL::FLAGS             }, \
        { _whirl_spawn_worldx_hi    }, \
        { _whirl_spawn_worldx_lo    }, \
        { #WHIRL::STARTY            }, \
        { #0                        }, \
        { #0                        }, \
        { #0                        }, \
        { #0                        }, \
        { #0                        }, \
        { _whirl_data_id            }

    done:
        pull a, x, y

    rts
.endproc

; @brief Spawns one whirl in the object pool
.export nsk_whirl_spawn
.proc nsk_whirl_spawn
    jsr _whirl_spawn

    rts
.endproc

; @brief Inits the whirl objects in the object pool
.export nsk_whirl_init
.proc nsk_whirl_init
    push x

    ldx #0

    loop:
        jsr _whirl_spawn

        inx
        cpx #WHIRL::DATA::MAX
        bne loop

    pull x

    rts
.endproc

; @brief Routine to clean up whirl object-specific data
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_whirl_clean
.proc nsk_whirl_clean
    push a, y

    ldy nsk_pool_data_id, x

    lda #WHIRL::DATA::FREE
    sta _whirl_data_used, y

    pull a, y

    rts
.endproc

; @brief Ticks whirl animation state
;
; @param[in] Y the index of the whirl data slot
.proc _whirl_animation_tick
    lda _whirl_data_timer, y
    beq advance

    sec
    sbc #1
    sta _whirl_data_timer, y
    jmp done

    advance:
        lda _whirl_data_state, y
        tax

        lda _whirl_data_frame, y
        clc
        adc #1
        cmp WHIRL::ANIMATION::FRAMES, x
        bcc set_frame

        lda _whirl_data_state, y
        cmp #WHIRL::STATE::KILL
        beq delete

        lda WHIRL::ANIMATION::NEXT_STATE, x
        sta _whirl_data_state, y
        tax

        lda #0

    set_frame:
        sta _whirl_data_frame, y

        lda WHIRL::ANIMATION::DURATION, x
        sta _whirl_data_timer, y

    done:
        rts

    delete:
        ldx _whirl_pool_index
        jsr nsk_fallingstar_spawn

        ldx _whirl_pool_index
        jsr nsk_starbit_spawn_burst

        ldx _whirl_pool_index
        lda nsk_pool_flags, x
        ora #POOL::FLAGS::DELETED
        sta nsk_pool_flags, x
        rts
.endproc

; @brief Stores the current candidate as the nearest whirl
;
; @param[in] X candidate pool index
.proc _whirl_nearest_set
    stx _whirl_best_index

    lda _whirl_distance_hi
    sta _whirl_best_distance_hi
    lda _whirl_distance_lo
    sta _whirl_best_distance_lo

    rts
.endproc

; @brief Updates the nearest whirl if the candidate is closer
;
; @param[in] X candidate pool index
.proc _whirl_nearest_update
    lda _whirl_best_index
    cmp #$ff
    beq set

    lda _whirl_distance_hi
    cmp _whirl_best_distance_hi
    bcc set
    bne done

    lda _whirl_distance_lo
    cmp _whirl_best_distance_lo
    bcs done

    set:
        jsr _whirl_nearest_set

    done:
        rts
.endproc

; @brief Checks the current whirl candidate for the right direction query
;
; @param[in] X candidate pool index
.proc _whirl_nearest_check_right
    lda nsk_pool_worldx_hi, x
    cmp nsk_whirl_query_x_hi
    bcc done
    bne distance

    lda nsk_pool_worldx_lo, x
    cmp nsk_whirl_query_x_lo
    bcc done

    distance:
        sec
        lda nsk_pool_worldx_lo, x
        sbc nsk_whirl_query_x_lo
        sta _whirl_distance_lo

        lda nsk_pool_worldx_hi, x
        sbc nsk_whirl_query_x_hi
        sta _whirl_distance_hi

        jsr _whirl_nearest_update

    done:
        rts
.endproc

; @brief Checks the current whirl candidate for the left direction query
;
; @param[in] X candidate pool index
.proc _whirl_nearest_check_left
    lda nsk_pool_worldx_hi, x
    cmp nsk_whirl_query_x_hi
    bcc distance
    bne done

    lda nsk_pool_worldx_lo, x
    cmp nsk_whirl_query_x_lo
    beq distance
    bcs done

    distance:
        sec
        lda nsk_whirl_query_x_lo
        sbc nsk_pool_worldx_lo, x
        sta _whirl_distance_lo

        lda nsk_whirl_query_x_hi
        sbc nsk_pool_worldx_hi, x
        sta _whirl_distance_hi

        jsr _whirl_nearest_update

    done:
        rts
.endproc

; @brief Finds the nearest whirl in the query direction
;
; @param[in] nsk_whirl_query_x_hi        Query X high byte
; @param[in] nsk_whirl_query_x_lo        Query X low byte
; @param[in] nsk_whirl_query_direction  LEFT=0, RIGHT=1
; @param[out] X current pool index of the nearest whirl, or $ff
.export nsk_whirl_nearest_find
.proc nsk_whirl_nearest_find
    push a, y

    lda #$ff
    sta _whirl_best_index

    ldx #0
    cpx nsk_pool_size
    beq done

    loop:
        stx _whirl_scan_index

        lda nsk_pool_object, x
        cmp #SPRITELIST::WHIRL
        bne next

        lda nsk_whirl_query_direction
        beq left

        jsr _whirl_nearest_check_right
        jmp next

        left:
            jsr _whirl_nearest_check_left

        next:
            ldx _whirl_scan_index
            inx
            cpx nsk_pool_size
            bne loop

    done:
        ldx _whirl_best_index

        pull a, y

        rts
.endproc

; @brief Routine to tick whirl-specific behavior
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_whirl_tick
.proc nsk_whirl_tick
    push a, x, y

    stx _whirl_pool_index

    ldy nsk_pool_data_id, x
    jsr _whirl_animation_tick

    pull a, x, y

    rts
.endproc

; @brief Routine to return the whirl collision box size
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_whirl_getbox
.proc nsk_whirl_getbox
    lda #(WHIRL::WIDTH * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEWIDTH)
    sta nsk_pool_box_width
    lda #(WHIRL::HEIGHT * NSK::SCREEN::SPRITES::MODE_8X8::SPRITEHEIGHT)
    sta nsk_pool_box_height

    rts
.endproc

; @brief Routine to handle whirl collision
;
; @param[in] X the index of the object in the nsk_pool_*
; @param[in] nsk_pool_collision_other other collided pool index
.export nsk_whirl_collision
.proc nsk_whirl_collision
    ldy nsk_pool_collision_other
    lda nsk_pool_object, y
    cmp #SPRITELIST::CHARACTER
    bne done

    ldy nsk_pool_data_id, x
    lda _whirl_data_state, y
    cmp #WHIRL::STATE::KILL
    beq done

    lda #WHIRL::STATE::KILL
    sta _whirl_data_state, y

    lda #0
    sta _whirl_data_frame, y

    lda #WHIRL::ANIMATION::KILL::DURATION
    sta _whirl_data_timer, y

    lda nsk_pool_flags, x
    and #($ff - POOL::FLAGS::COLLISION)
    sta nsk_pool_flags, x

    done:
        rts
.endproc

; @brief Selects the current whirl frame sprite table
;
; @param[in] Y the index of the whirl data slot
.proc _whirl_frame_select
    lda _whirl_data_state, y
    asl
    tax

    lda WHIRL::ANIMATION::TABLE + 0, x
    sta _whirl_animation_ptr + 0
    lda WHIRL::ANIMATION::TABLE + 1, x
    sta _whirl_animation_ptr + 1

    lda _whirl_data_frame, y
    asl
    tay

    lda (_whirl_animation_ptr), y
    sta _whirl_frame_ptr + 0
    iny
    lda (_whirl_animation_ptr), y
    sta _whirl_frame_ptr + 1

    rts
.endproc

; @brief Draws the selected whirl frame
.proc _whirl_frame_draw
    ldx _whirl_pool_index
    ldy #0

    loop:
        lda (_whirl_frame_ptr), y
        sta _whirl_sprite

        clc
        lda nsk_pool_screenx
        adc WHIRL::POSX::TABLE, y
        bcs next
        sta _whirl_screenx

        clc
        lda nsk_pool_worldy_lo, x
        adc WHIRL::POSY::TABLE, y
        sta _whirl_screeny

        nsk_sprite_draw \
            { _whirl_sprite     }, \
            { #WHIRL::ATTRS     }, \
            { #WHIRL::PALETTE   }, \
            { _whirl_screenx    }, \
            { _whirl_screeny    }

        next:
        iny
        cpy #WHIRL::COUNT
        bne loop

    rts
.endproc

; @brief Routine to draw the whirl
;
; @param[in] X the index of the object in the nsk_pool_*
.export nsk_whirl_draw
.proc nsk_whirl_draw
    push a, x, y

    stx _whirl_pool_index

    ldy nsk_pool_data_id, x
    jsr _whirl_frame_select

    jsr _whirl_frame_draw

    pull a, x, y

    rts
.endproc

.endif
