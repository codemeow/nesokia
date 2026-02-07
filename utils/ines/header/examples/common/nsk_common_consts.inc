; @file nsk_common_consts.inc
; @brief Include module for the NES constants
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_COMMON_CONSTS__
::__NSK_COMMON_CONSTS__ = 1

.linecont +

; @brief Global project namespace
.scope NSK

    ; @brief Screen related sizes
    .scope SCREEN
        ; @brief Tilemap info
        .scope TILEMAP
            WIDTH  = $20 ; Screen width in tiles
            HEIGHT = $1e ; Screen height in tiles
        .endscope

        ; @brief Tiles info
        .scope TILE
            WIDTH    = $08 ; Tile width in pixels
            HEIGHT   = $08 ; Tile height in pixels
        .endscope

        ; @brief Attributes info
        .scope ATTR
            ; @brief Byte sizes
            .scope BYTE
                WIDTH   = $08 ; Attribute table width in bytes
                HEIGHT  = $08 ; Attribute table height in bytes
            .endscope
        .endscope

        WIDTH  = \
            NSK::SCREEN::TILEMAP::WIDTH *  \
            NSK::SCREEN::TILE::WIDTH       ; Screen size in pixels
        HEIGHT = \
            NSK::SCREEN::TILEMAP::HEIGHT * \
            NSK::SCREEN::TILE::HEIGHT      ; Screen size in pixels

        ; @brief Sprites info
        .scope SPRITES
            ; @brief 8x8 mode sizes
            .scope MODE_8X8
                SPRITEWIDTH  = $08 ; Sprite width in pixels
                SPRITEHEIGHT = $08 ; Sprite height in pixels
            .endscope

            ; @brief 8x16 mode sizes
            .scope MODE_8X16
                SPRITEWIDTH  = $08 ; Sprite width in pixels
                SPRITEHEIGHT = $10 ; Sprite height in pixels
            .endscope
        .endscope
    .endscope

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

                ; @brief DMC bitflags
                .scope BITS
                    ; @brief IRQ bitflags
                    .scope IRQ
                        ; @brief RATE bitflags
                        .scope RATE
                            ; @brief NTSC rate bitflags
                            .scope NTSC
                                R_428 = %00000000 ; 428 CPU cycles, 4181.71 Hz
                                R_380 = %00000001 ; 380 CPU cycles, 4709.93 Hz
                                R_340 = %00000010 ; 340 CPU cycles, 5264.04 Hz
                                R_320 = %00000011 ; 320 CPU cycles, 5593.04 Hz
                                R_286 = %00000100 ; 286 CPU cycles, 6257.95 Hz
                                R_254 = %00000101 ; 254 CPU cycles, 7046.35 Hz
                                R_226 = %00000110 ; 226 CPU cycles, 7919.35 Hz
                                R_214 = %00000111 ; 214 CPU cycles, 8363.43 Hz
                                R_190 = %00001000 ; 190 CPU cycles, 9419.86 Hz
                                R_160 = %00001001 ; 160 CPU cycles, 11186.08 Hz
                                R_142 = %00001010 ; 142 CPU cycles, 12604.04 Hz
                                R_128 = %00001011 ; 128 CPU cycles, 13982.60 Hz
                                R_106 = %00001100 ; 106 CPU cycles, 16884.65 Hz
                                R_84  = %00001101 ; 84 CPU cycles, 21306.82 Hz
                                R_72  = %00001110 ; 72 CPU cycles, 24857.96 Hz
                                R_54  = %00001111 ; 54 CPU cycles, 33143.94 Hz
                            .endscope

                            ; @brief PAL rate bitflags
                            .scope PAL
                                R_398  = %00000000 ; 398 CPU cycles, 4177.40 Hz
                                R_354  = %00000001 ; 354 CPU cycles, 4696.63 Hz
                                R_316  = %00000010 ; 316 CPU cycles, 5261.41 Hz
                                R_298  = %00000011 ; 298 CPU cycles, 5579.22 Hz
                                R_276  = %00000100 ; 276 CPU cycles, 6023.94 Hz
                                R_236  = %00000101 ; 236 CPU cycles, 7044.94 Hz
                                R_210  = %00000110 ; 210 CPU cycles, 7917.18 Hz
                                R_198  = %00000111 ; 198 CPU cycles, 8397.01 Hz
                                R_176  = %00001000 ; 176 CPU cycles, 9446.63 Hz
                                R_148  = %00001001 ; 148 CPU cycles, 11233.83 Hz
                                R_132  = %00001010 ; 132 CPU cycles, 12595.51 Hz
                                R_118  = %00001011 ; 118 CPU cycles, 14089.89 Hz
                                R_98   = %00001100 ; 98 CPU cycles, 16965.38 Hz
                                R_78   = %00001101 ; 78 CPU cycles, 21315.47 Hz
                                R_66   = %00001110 ; 66 CPU cycles, 25191.02 Hz
                                R_50   = %00001111 ; 50 CPU cycles, 33252.14 Hz
                            .endscope

                        .endscope

                        LOOP_OFF    = %00000000 ; Disable looping
                        LOOP_ON     = %01000000 ; Enable looping

                        IRQ_DISABLE = %00000000 ; Disable IRQ
                        IRQ_ENABLE  = %10000000 ; Enable IRQ
                    .endscope
                .endscope
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
                    MODE_4STEP          = %00000000 ; Set 4-step mode
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

            ; Sizes in bytes
            .scope SIZE
                TABLE = $03c0   ; Nametable size
                ATTRS = $40     ; Attribute table size
            .endscope
        .endscope

        ; @brief Palettes mappings
        .scope PALETTE
            TILES   = $3f00 ; Tiles' palette
            SPRITES = $3f10 ; Sprites' palette

            SIZE    = $10   ; Single palette size in bytes
        .endscope

        ; @brief Sprites related constants for OAM
        .scope SPRITES
            ; @brief Max sprites slots
            MAXCOUNT = 64

            ; @brief Attributes
            .scope ATTRIBUTE
                PALETTEMASK = %11 ; First two bits represent the palette

                ; @brief Sprite priority
                .scope PRIORITY
                    FOREGROUND = 0 ; Above the background
                    BACKGROUND = 1 ; Under the background
                .endscope

                ; @brief Flip attributes
                .scope FLIP

                    ; @brief Horizontally
                    .scope HOR
                        NO     = 0 ; Do not flip
                        YES    = 1 ; Flip
                    .endscope

                    ; @brief Vertically
                    .scope VER
                        NO     = 0 ; Do not flip
                        YES    = 1 ; Flip
                    .endscope
                .endscope
            .endscope
        .endscope
    .endscope
.endscope

.endif