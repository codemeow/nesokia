; @file nsk_common_consts.inc
; @brief Include module for the NES constants
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_COMMON_CONSTS__
::__NSK_COMMON_CONSTS__ = 1

; @brief Global project namespace
.scope NSK

    ; @brief CPU-related constants
    .scope CPU
        ; @brief RAM memory segment
        .scope RAM
            ZEROPAGE  = $0000 ; Zeropage start address
            STACK     = $0100 ; Stack start address
            RAMSTART  = $0200 ; Common RAM start address
            OAMBUFFER = NSK::CPU::RAM::RAMSTART ; Common OAM buffer position
        .endscope

        ; @brief PPU control registers
        .scope PPU
            PPUCTRL   = $2000 ; PPU control register
            PPUMASK   = $2001 ; PPU mask register
            PPUSTATUS = $2002 ; PPU status register
            OAMADDR   = $2003 ; PPU OAM address
            OAMDATA   = $2004 ; PPU OAM data register
            PPUSCROLL = $2005 ; PPU scroll register
            PPUADDR   = $2006 ; PPU write address register
            PPUDATA   = $2007 ; PPU data register
            OAMDMA    = $4014 ; PPU OAM DMA register
        .endscope

        ; @brief APU control registers
        .scope APU
            ; @brief Pulse 1 controls
            .scope PULSE1
                VOLUME = $4000 ; Volume/envelope control
                SWEEP  = $4001 ; Sweep unit control
                LO     = $4002 ; Low byte storage
                HI     = $4003 ; High byte storage
            .endscope

            ; @brief Pulse 2 controls
            .scope PULSE2
                VOLUME = $4004 ; Volume/envelope control
                SWEEP  = $4005 ; Sweep unit control
                LO     = $4006 ; Low byte storage
                HI     = $4007 ; High byte storage
            .endscope

            ; @brief Triangle wave control
            .scope TRIANGLE
                LINEAR = $4008 ; Linear counter
                LO     = $400a ; Low byte storage
                HI     = $400b ; Length/high byte storage
            .endscope

            ; @brief Noise channel control
            .scope NOISE
                VOLUME = $400c ; Volume/envelope control
                LO     = $400e ; Mode and period
                HI     = $400f ; Length counter
            .endscope

            ; @brief Delta modulation channel control
            .scope DMC
                FREQUENCY = $4010 ; Frequency control
                IRQ       = $4010 ; IRQ control
                RAW       = $4011 ; Raw DAC value
                START     = $4012 ; Sample address
                LENGTH    = $4013 ; Sample length
            .endscope

            SOUNDCHANNEL  = $4015 ; Channel selection and status
            FRAMECOUNT    = $4017 ; Frame counter
            IRQ           = $4017 ; APU IRQ

            ; @brief Bitflags for APU registers
            .scope BITS
                ; @brief APU FRAMECOUNTER/IRQ bit flags
                .scope FRAMECOUNTER
                    IRQ_ENABLE          = %00000000 ; Enable IRQ
                    IRQ_DISABLE         = %01000000 ; Disable IRQ
                    MODE_4STEP          = %00000000 ; Set4-step mode
                    MODE_5STEP          = %10000000 ; Set 5-step mode
                .endscope
            .endscope
        .endscope

        ; @brief Controller ports and strobe registers
        .scope INPUT
            JOYSTROBE = $4016 ; Strobe register
            JOY1      = $4016 ; Joystick 1 port
            JOY2      = $4017 ; Joystick 2 port
        .endscope

        ; @brief Common WRAM and PRG banks positions
        .scope CARTRIDGE
            WRAM  = $6000 ; WRAM base address
            ROM1  = $8000 ; ROM bank 1 address
            ROM2  = $a000 ; ROM bank 2 address
            ROM3  = $c000 ; ROM bank 3 address
            ROM4  = $e000 ; ROM bank 4 address
        .endscope

        ; @brief Interrupt vectors
        .scope VECTORS
            NMI   = $fffa ; NMI interrupt handler address
            RESET = $fffc ; RESET interrupt handler address
            IRQ   = $fffe ; IRQ interrupt handler address
        .endscope
    .endscope

    ; @brief PPU-related constants
    .scope PPU
        ; @brief Pattern tables
        .scope PATTERN
            TABLE0  = $0000 ; Pattern table 0 (left)
            TABLE1  = $1000 ; Pattern table 1 (right)
        .endscope

        ; @brief Nametables data
        .scope NAMETABLE
            TABLE0  = $2000 ; Nametable 0
            ATTR0   = $23c0 ; Nametable's 0 attributes grid

            TABLE1  = $2400 ; Nametable 1
            ATTR1   = $27c0 ; Nametable's 1 attributes grid

            TABLE2  = $2800 ; Nametable 2
            ATTR2   = $2bc0 ; Nametable's 2 attributes grid

            TABLE3  = $2c00 ; Nametable 3
            ATTR3   = $2fc0 ; Nametable's 3 attributes grid
        .endscope

        ; @brief Palettes mappings
        .scope PALETTE
            TILES   = $3f00 ; Tiles' palette
            SPRITES = $3f10 ; Sprites' palette
        .endscope

        ; @brief Bitflags for PPU registers
        .scope BITS
            ; @brief PPUCTRL bit flags
            .scope PPUCTRL
                NAMETABLE_2000    = %00000000 ; Base nametable is $2000
                NAMETABLE_2400    = %00000001 ; Base nametable is $2400
                NAMETABLE_2800    = %00000010 ; Base nametable is $2800
                NAMETABLE_2c00    = %00000011 ; Base nametable is $2c00

                VRAM_INC_1        = %00000000 ; VRAM increments by 1
                VRAM_INC_32       = %00000100 ; VRAM increments by 32

                SPRITE_TABLE_0000 = %00000000 ; Sprite pattern table is $0000 (8x8 only)
                SPRITE_TABLE_1000 = %00001000 ; Sprite pattern table is $1000 (8x8 only)

                TILES_TABLE_0000  = %00000000 ; Background pattern table is $0000
                TILES_TABLE_1000  = %00010000 ; Background pattern table is $1000

                SPRITE_8x8        = %00000000 ; Sprite size is 8x8
                SPRITE_8x16       = %00100000 ; Sprite size is 8x16

                PPU_SLAVE         = %00000000 ; PPU reads backdrop from EXT pins
                PPU_MASTER        = %01000000 ; PPU emits backdrop to EXT pins

                NMI_DISABLE       = %00000000 ; NMI interrupt is disabled
                NMI_ENABLE        = %10000000 ; NMI interrupt is enabled
                NMI_BIT           = %10000000 ; NMI interrupt bitmask
            .endscope

            ; @brief PPUMASK bit flags
            .scope PPUMASK
                GRAYSCALE_OFF       = %00000000 ; Grayscale mode is disabled
                GRAYSCALE_ON        = %00000001 ; Grayscale mode is enabled

                LEFTMOST_BACK_OFF   = %00000000 ; Hide leftmost 8px background tiles
                LEFTMOST_BACK_ON    = %00000010 ; Show leftmost 8px background tiles
                LEFTMOST_SPRITE_OFF = %00000000 ; Hide leftmost 8px sprites
                LEFTMOST_SPRITE_ON  = %00000100 ; Show leftmost 8px sprites

                RENDER_BACK_OFF     = %00000000 ; Disable background rendering
                RENDER_BACK_ON      = %00001000 ; Enable background rendering
                RENDER_BACK_BIT     = %00001000 ; Bitmask of the background rendering
                RENDER_SPRITES_OFF  = %00000000 ; Disable sprites rendering
                RENDER_SPRITES_ON   = %00010000 ; Enable sprites rendering
                RENDER_SPRITES_BIT  = %00010000 ; Bitmask of the sprites rendering

                RED_NORMAL          = %00000000 ; Normal representation of red
                RED_EMPHASIZED      = %00100000 ; Emphasize red color
                GREEN_NORMAL        = %00000000 ; Normal representation of green
                GREEN_EMPHASIZED    = %01000000 ; Emphasize green color
                BLUE_NORMAL         = %00000000 ; Normal representation of blue
                BLUE_EMPHASIZED     = %10000000 ; Emphasize blue color
            .endscope

            ; @brief PPUSTATUS bit flags
            .scope PPUSTATUS
                OPENBUS_MASK        = %00011111 ; Open bus values mask
                SPRITE_OVERFLOW     = %00100000 ; Sprite overflow bit flag
                SPRITE_0_HIT        = %01000000 ; Sprite 0 hit flag
                VBLANK_FLAG         = %10000000 ; VBLANK flag
            .endscope
        .endscope
    .endscope
.endscope

.endif