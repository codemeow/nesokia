; @file nsk_header_flags6.inc
; @brief Include module for the compilation of the flags for the 7th header byte
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_FLAGS7__
::__NSK_HEADER_FLAGS7__ = 1

.linecont +

.include "nsk_header_mapper_consts.inc"

; @def NSK_HEADER_ISNES20
; @brief Marker telling the header builder to emit NES 2.0 format bits.
;
; When set to 1, the Flags 7 "NES 2.0" indicator (bits 2–3) will be
; encoded accordingly by @_convert_nes20 and written by @ref nsk_header_flags7.
::NSK_HEADER_ISNES20 = 1

; @def NSK_HEADER_CONSOLE_EXTENDED
; @brief Sentinel value for "extended console type" in Flags 7
;
; Sets the console field in Flags 7 to the extended selector
; Extended console identification is then provided via additional bytes/fields
; as defined by the NES 2.0 specification.
::NSK_HEADER_CONSOLE_EXTENDED = $03

; @macro _convert_console
; @brief Normalize a console identifier into Flags 7 (bits 0–1).
;
; Passes through known base console codes (Famicom/NES/Dendy/VS/PlayChoice).
; Maps any supported "extended" console family (clones/VT/UMC/network, etc.)
; to @ref NSK_HEADER_CONSOLE_EXTENDED.
;
; @param    input     One of `nsk::console::xxx`.
; @param    out_bits  Symbol assigned to the resulting 2-bit code
;
; @error Emits an error when @p input is not a recognized console identifier.
.macro _convert_console input, out_bits
    .local convert
    convert:
        .if                                                    \
            input = nsk::console::famicom                   || \
            input = nsk::console::NES                       || \
            input = nsk::console::dendy                     || \
            input = nsk::console::nintendo_vs_system        || \
            input = nsk::console::nintendo_playchoice_10

            out_bits = input
        .elseif                                                \
            input = nsk::console::famiclone_decimal         || \
            input = nsk::console::famicom_epsm              || \
            input = nsk::console::VR_Technology_VT01        || \
            input = nsk::console::VR_Technology_VT02        || \
            input = nsk::console::VR_Technology_VT03        || \
            input = nsk::console::VR_Technology_VT09        || \
            input = nsk::console::VR_Technology_VT32        || \
            input = nsk::console::VR_Technology_VT369       || \
            input = nsk::console::UMC_UM6578                || \
            input = nsk::console::famicom_network_system    || \

            out_bits = ::NSK_HEADER_CONSOLE_EXTENDED
        .elseif
            .error "Invalid value: NSK_HEADER_CONSOLE_TYPE"
        .endif
.endmacro

; @macro _convert_nes20
; @brief Encode the NES 2.0 indicator bits for Flags 7
;
; Maps a boolean-like input to the 2-bit NES 2.0 signature
;
; @param    input     1 to mark NES 2.0, 0 for iNES.
; @param    out_bits  Symbol assigned to the resulting 2-bit pattern.
;
; @error Emits an error when @p input is ∉ {0, 1}
.macro _convert_nes20 input, out_bits
    .local convert
    convert:
        .if   input = 1
            out_bits = %10
        .elseif input = 0
            out_bits = %00
        .elseif
            .error "Invalid value: NSK_HEADER_ISNES20"
        .endif
.endmacro

; @macro _convert_mapperid_mid
; @brief Extract the middle nibble of a NES 2.0 mapper ID for Flags 7.
;
; @param    input     Mapper ID (0..4095).
; @param    out_nib   Symbol assigned to resulting nibble
;
; @error Emits a error if @p input > 4095.
.macro _convert_mapper_mid input, out_nib
    .local convert
    convert:
        .if   input >= 0 && input <= ::_NSK_HEADER_MAPPER_MAXID
            out_nib = (input >> 4) & $0f
        .elseif
            .error "Invalid value: NSK_HEADER_MAPPER_ID"
        .endif
.endmacro

; @macro nsk_header_flags7
; @brief Build header Flags 7 from console type, NES2 marker, and mapper ID.
;
; @param    console   Console identifier (One of `nsk::console::xxx`).
; @param    mapperid  NES 2.0 mapper ID (0..4095).
.macro nsk_header_flags7 console, mapperid
    .local locals
    locals:
        .local bits_console
        .local bits_nes20
        .local nib_mapper

    .local set
    set:
        _convert_console    console,                bits_console
        _convert_nes20      ::NSK_HEADER_ISNES20,   bits_nes20
        _convert_mapper_mid mapperid,               nib_mapper

    .local write
    write:
        .byte                        \
            (bits_console        ) | \
            (bits_nes20     <<  2) | \
            (nib_mapper     <<  4)
.endmacro

.endif
