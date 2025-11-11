; @file nsk_header_flags6.inc
; @brief Include module for the compilation of the flags for the 6th header byte
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_FLAGS6__
::__NSK_HEADER_FLAGS6__ = 1

.linecont +

.include "nsk_header_mapper_consts.inc"

; @macro _convert_nametable_hardwired
; @brief Convert a hardwired nametable arrangement to the Flags 6 bit value.
;
; @param    input     One of nsk::nametable::layout::xxx.
; @param    out_bit   Symbol assigned to the resulting bit (0 or 1).
;
; @error Emits a error if @p input is not a supported layout constant.
.macro _convert_nametable_hardwired input, out_bit
    .if                                                   \
        input = NSK::NM_LAYOUT::VERTICAL_ARRANGEMENT   || \
        input = NSK::NM_LAYOUT::HORIZONTAL_MIRRORING   || \
        input = NSK::NM_LAYOUT::MAPPER_CONTROLLED

        out_bit = 0
    .elseif                                               \
        input = NSK::NM_LAYOUT::HORIZONTAL_ARRANGEMENT || \
        input = NSK::NM_LAYOUT::VERTICAL_MIRRORING

        out_bit = 1
    .elseif
        .error .sprintf("Invalid value: NSK_HEADER_NAMETABLE_HARDWIRED: %d", input)
    .endif
.endmacro

; @macro _convert_battery
; @brief Convert battery presence to the Flags 6 bit value.
;
; @param    input     One of nsk::battery::xxx
; @param    out_bit   Symbol assigned to the resulting bit (0 or 1).
;
; @error Emits a error if @p input is not recognized.
.macro _convert_battery input, out_bit
    .if   input = NSK::BATTERY::NOT_PRESENT
        out_bit = 0
    .elseif input = NSK::BATTERY::PRESENT
        out_bit = 1
    .elseif
        .error .sprintf("Invalid value: NSK_HEADER_BATTERY: %d", input)
    .endif
.endmacro

; @macro _convert_trainer
; @brief Convert trainer presence to the Flags 6 bit value.
;
; @param    input     One of nsk::trainer::xxx
; @param    out_bit   Symbol assigned to the resulting bit (0 or 1).
;
; @error Emits a error if @p input is not recognized.
.macro _convert_trainer input, out_bit
    .if   input = NSK::TRAINER::NOT_PRESENT
        out_bit = 0
    .elseif input = NSK::TRAINER::PRESENT
        out_bit = 1
    .elseif
        .error .sprintf("Invalid value: NSK_HEADER_TRAINER: %d", input)
    .endif
.endmacro

; @macro _convert_nametable_alternative
; @brief Convert alternative mirroring mode to the Flags 6 bit value.
;
; @param    input     One of nsk::nametable::mode::xxx
; @param    out_bit   Symbol assigned to the resulting bit (0 or 1).
;
; @error Emits a error if @p input is not a supported mode constant.
.macro _convert_nametable_alternative input, out_bit
    .if   input = NSK::NM_ALTERNATIVE::NORMAL
        out_bit = 0
    .elseif input = NSK::NM_ALTERNATIVE::ALTERNATIVE
        out_bit = 1
    .elseif
        .error .sprintf("Invalid value: NSK_HEADER_NAMETABLE_ALTERNATIVE: %d", input)
    .endif
.endmacro

; @macro _convert_mapperid_low
; @brief Extract the low nibble of a NES 2.0 mapper ID for Flags 6.
;
; @param    input     Mapper ID (0..4095). 
; @param    out_nib   Symbol assigned to resulting nibble
;
; @error Emits a error if @p input > 4095.
.macro _convert_mapperid_low input, out_nib
    .if   input >= 0 && input <= ::_NSK_HEADER_MAPPER_MAXID
        out_nib = input & $0f
    .else
        .error .sprintf("Invalid value: NSK_HEADER_MAPPER_ID: %d", input)
    .endif
.endmacro

; @macro nsk_header_flags6
; @brief Build iNES/NES 2.0 header Flags 6 byte.
;
; Composes the following bits into header byte 6:
;   bit 0 : Nametable hardwired arrangement
;           (0 = vertical_arrangement, 1 = horizontal_arrangement)
;   bit 1 : Battery-backed RAM (0 = absent, 1 = present)
;   bit 2 : Trainer present       (0 = absent, 1 = present)
;   bit 3 : Alternative mirroring (0 = normal, 1 = alternative)
;   bits4..7 : Mapper ID low nibble (mapperid & $0F)
;
; @param    nametablehw  nsk::nametable::layout::xxx
; @param    battery      nsk::battery::xxx
; @param    trainer      nsk::trainer::xxx
; @param    nametablealt nsk::nametable::mode::xxx
; @param    mapperid     NES 2.0 mapper ID (0..4095)
.macro nsk_header_flags6 nametablehw, battery, trainer, nametablealt, mapperid
    ; locals:
    .local bit_nametablehw
    .local bit_battery
    .local bit_trainer
    .local bit_nametablealt
    .local nib_mapper

    ; set:
    _convert_battery                battery,        bit_battery
    _convert_mapperid_low           mapperid,       nib_mapper
    _convert_nametable_alternative  nametablealt,   bit_nametablealt
    _convert_nametable_hardwired    nametablehw,    bit_nametablehw
    _convert_trainer                trainer,        bit_trainer

    ; write:
    .byte                            \
        (bit_nametablehw         ) | \
        (bit_battery        <<  1) | \
        (bit_trainer        <<  2) | \
        (bit_nametablealt   <<  3) | \
        (nib_mapper         <<  4)
.endmacro

.endif
