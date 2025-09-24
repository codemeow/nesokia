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

.endscope

.endif
