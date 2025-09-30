; @file nsk_header_consts.inc
; @brief Constant definitions and enumerations for NES 2.0 headers.
;
; Provides named values for flags, and configuration
; options referenced in `nsk_header_config.inc`.
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_CONSTS__
::__NSK_HEADER_CONSTS__ = 1

; @enum nsk
; @brief Project-wide prefix
.scope nsk

    ; @enum nametable
    ; @brief Nametable settings
    .scope nametable

        ; @enum nametable::layout
        ; @brief Nametable arrangement / mirroring modes.
        ;
        ; Nametable mirroring controls how the PPU maps its 2 KiB of VRAM
        ; into the four logical nametables ($2000, $2400, $2800, $2C00).
        ; 
        ; - Vertical mirroring: NT0 = $2000, NT1 = $2400, both mirrored into
        ;   $2800/$2C00. Two unique vertical columns of 32×30 tiles.
        ; - Horizontal mirroring: NT0 = $2000, NT1 = $2800, both mirrored into
        ;   $2400/$2C00. Two unique horizontal rows of 32×30 tiles.
        ;
        ; Effectively, mirroring determines whether VRAM provides
        ; two screens stacked vertically or horizontally, with the
        ; other two screens being duplicates.
        ;
        ; Some boards are exceptions: certain cartridges wire only a single
        ; physical nametable, which is mirrored into all four logical slots.
        ; On the other end, advanced mappers can provide four fully
        ; independent nametables, giving the PPU complete flexibility
        ; without any mirroring.
        ; 
        ; See https://www.nesdev.org/wiki/Mirroring#Nametable_Mirroring for
        ; more info
        .scope layout
            ; @var vertical_arrangement
            ; @brief Vertical arrangement
            vertical_arrangement    = 0

            ; @var horizontal_arrangement
            ; @brief Horizontal arrangement
            horizontal_arrangement  = 1

            ; @var vertical_mirroring
            ; @brief Standard term: vertical mirroring
            vertical_mirroring      = nsk::nametable::alignment::horizontal_arrangement

            ; @var horizontal_mirroring
            ; @brief Standard term: horizontal mirroring
            horizontal_mirroring    = nsk::nametable::alignment::vertical_arrangement
        .endscope


        ; @enum nametable::mode
        ; @brief Alternative mirroring flag.
        .scope mode
            ; @var normal
            ; @brief Normal mirroring configuration.
            normal                  = 0

            ; @var alternative
            ; @brief Alternative mirroring configuration.
            alternative             = 1
        .endscope

    .endscope


    ; @enum battery
    ; @brief Presence of battery-backed RAM on the cartridge.
    .scope battery
        ; @var absent
        ; @brief Cartridge has no battery-backed RAM.
        absent                      = 0

        ; @var present
        ; @brief Cartridge includes battery-backed RAM.
        present                     = 1
    .endscope


    ; @enum trainer
    ; @brief Presence of a 512-byte trainer at $7000–$71FF.
    .scope trainer
        ; @var absent
        ; @brief Cartridge does not include a trainer.
        absent                      = 0

        ; @var present
        ; @brief Cartridge includes a trainer section.
        present                     = 1
    .endscope

    ; @enum console
    ; @brief Console type identifiers for NES 2.0 header Flags 7.
    ;
    ; Encodes the target hardware family of the ROM image. 
    .scope console

        ; @var famicom
        ; @brief Nintendo Family Computer (Famicom).
        famicom                     = $00

        ; @var NES
        ; @brief Nintendo Entertainment System (NTSC/PAL). Alias of @ref console::famicom.
        NES                         = nsk::console::famicom

        ; @var dendy
        ; @brief Dendy (Russian NES clone). Alias of @ref console::famicom.
        dendy                       = nsk::console::famicom

        ; @var nintendo_vs_system
        ; @brief Nintendo VS. System arcade hardware.
        nintendo_vs_system          = $01

        ; @var nintendo_playchoice_10
        ; @brief Nintendo PlayChoice-10 arcade system.
        nintendo_playchoice_10      = $02

        ; @var famiclone_decimal
        ; @brief Generic famiclone with decimal-based PPU.
        famiclone_decimal           = $03

        ; @var famicom_epsm
        ; @brief Famicom clone with extended sound (EPSM).
        famicom_epsm                = $04

        ; @var VR_Technology_VT01
        ; @brief VT01 SoC (basic NOAC clone).
        VR_Technology_VT01          = $05

        ; @var VR_Technology_VT02
        ; @brief VT02 SoC.
        VR_Technology_VT02          = $06

        ; @var VR_Technology_VT03
        ; @brief VT03 SoC.
        VR_Technology_VT03          = $07

        ; @var VR_Technology_VT09
        ; @brief VT09 SoC.
        VR_Technology_VT09          = $08

        ; @var VR_Technology_VT32
        ; @brief VT32 SoC.
        VR_Technology_VT32          = $09

        ; @var VR_Technology_VT369
        ; @brief VT369 SoC.
        VR_Technology_VT369         = $0a

        ; @var UMC_UM6578
        ; @brief UMC UM6578 SoC.
        UMC_UM6578                  = $0b

        ; @var famicom_network_system
        ; @brief Famicom Network System (Japan-only add-on).
        famicom_network_system      = $0c

        ; @enum console::vs
        ; @brief VS console additional constants
        .scope vs

            ; @enum console::vs::ppu
            ; @brief VS console PPU types
            .scope ppu
                ; @var RP2C03
                ; @brief RP2C03 PPU chip
                RP2C03 = $00

                ; @var RC2C03
                ; @brief RC2C03 PPU chip
                RC2C03 = nsk::console::vs::ppu::RP2C03

                ; @enum console::vs::ppu::RP2C04
                ; @brief RP2C04 PPU chip
                .scope RP2C04
                    ; @var rev1
                    ; @brief Revision 1
                    rev1 = $02

                    ; @var rev2
                    ; @brief Revision 2
                    rev2 = $03

                    ; @var rev3
                    ; @brief Revision 3
                    rev3 = $04

                    ; @var rev4
                    ; @brief Revision 4
                    rev4 = $05
                .endscope

                ; @enum RC2C05
                ; @brief RC2C05 PPU chip
                .scope RC2C05
                    ; @var rev1
                    ; @brief Revision 1
                    rev1 = $08

                    ; @var rev2
                    ; @brief Revision 2
                    rev2 = $09

                    ; @var rev3
                    ; @brief Revision 3
                    rev3 = $0a

                    ; @var rev4
                    ; @brief Revision 4
                    rev4 = $0b
                .endscope
            .endscope

            ; @enum console::vs::hardware
            ; @enum VS console hardware types
            .scope hardware

                ; @enum console::vs::hardware::unisystem
                ; @enum Unisystem hardware types
                .scope unisystem
                    ; @var normal
                    ; @brief Standard Vs. Unisystem hardware (0).
                    normal              = $00

                    ; @var RBI_baseball
                    ; @brief Vs. Unisystem with RBI Baseball protection
                    RBI_baseball        = $01

                    ; @var TKO_boxing
                    ; @brief Vs. Unisystem with TKO Boxing protection
                    TKO_boxing          = $02

                    ; @var Super_Xevious
                    ; @brief Vs. Unisystem with Super Xevious protection
                    super_xevious       = $03

                    ; @var ice_climber_japan
                    ; @brief Vs. Unisystem with Ice Climber Japan protection
                    ice_climber_japan   = $04
                .endscope

                ; @enum console::vs::hardware::dualsystem
                ; @enum Dualsystem hardware types
                .scope dualsystem
                    ; @var normal
                    ; @brief Standard Vs. DualSystem hardware 
                    normal                  = $05

                    ; @var raid_on_bungeling_bay
                    ; @brief Vs. DualSystem with Raid on Bungeling Bay protection
                    raid_on_bungeling_bay   = $06
                .endscope
            .endscope

        .endscope

    .endscope

    ; @enum nsk::region
    ; @brief Region and PPU identifiers for NES 2.0 header configuration.
    ;
    ; Encodes the intended video standard / PPU variant used by the ROM.
    ; These values inform the emulator or hardware which timing and
    ; region-specific behavior to apply.
    ;
    ; Multiple aliases are provided for human readability, all resolving
    ; to the same underlying constant.
    .scope region
        ; @var RP2C02
        ; @brief NTSC PPU used in Japan and North America
        ; 
        ; Licensed NTSC 60 Hz 
        RP2C02          = $00
        ; @var NTSC
        ; @brief Alias of @ref region::RP2C02
        NTSC            = nsk::region::RP2C02

        ; @var North_America
        ; @brief Alias of @ref region::RP2C02
        North_America   = nsk::region::RP2C02

        ; @var Japan
        ; @brief Alias of @ref region::RP2C02
        Japan           = nsk::region::RP2C02

        ; @var South_Korea
        ; @brief Alias of @ref region::RP2C02
        South_Korea     = nsk::region::RP2C02

        ; @var Taiwan
        ; @brief Alias of @ref region::RP2C02
        Taiwan          = nsk::region::RP2C02


        ; @var RP2C07
        ; @brief PAL PPU used in Western Europe and Australia
        ;
        ; Licensed PAL 50 Hz 
        RP2C07          = $01

        ; @var PAL
        ; @brief Alias of @ref region::RP2C07
        PAL             = nsk::region::RP2C07

        ; @var Western_Europe
        ; @brief Alias of @ref region::RP2C07
        Western_Europe  = nsk::region::RP2C07

        ; @var Australia
        ; @brief Alias of @ref region::RP2C07
        Australia       = nsk::region::RP2C07


        ; @var Multiple
        ; @brief Multiregion / dual-PPU cartridge support
        Multiple        = $02


        ; @var UA6538
        ; @brief Dendy-compatible PAL clone PPU, common in E.Europe, Russia, etc
        UA6538          = $03
    
        ; @var Dendy
        ; @brief Alias of @ref region::UA6538
        ;
        ; Unlicensed hybrid with 50 Hz PAL CPU and 60 Hz NTSC PPU
        Dendy           = nsk::region::UA6538
    
        ; @var Eastern_Europe
        ; @brief Alias of @ref region::UA6538
        Eastern_Europe  = nsk::region::UA6538
    
        ; @var Russia
        ; @brief Alias of @ref region::UA6538
        Russia          = nsk::region::UA6538
    
        ; @var Mainland_China
        ; @brief Alias of @ref region::UA6538
        Mainland_China  = nsk::region::UA6538
    
        ; @var India
        ; @brief Alias of @ref region::UA6538
        India           = nsk::region::UA6538
    
        ; @var Africa
        ; @brief Alias of @ref region::UA6538
        Africa          = nsk::region::UA6538
    .endscope

.endscope

.endif
