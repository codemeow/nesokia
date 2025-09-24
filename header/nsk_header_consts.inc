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

    .endscope


.endscope

.endif
