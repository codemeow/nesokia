; @file nsk_header_mapper.inc
; @brief Include module for the mapper byte calculations
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_MAPPER__
::__NSK_HEADER_MAPPER__ = 1

.linecont +

.include "nsk_header_mapper_consts.inc"

; @macro _convert_mapperid_hi
; @brief Extract the high nibble of a NES 2.0 mapper ID.
;
; Validates the 12-bit mapper ID range (0..@ref _NSK_HEADER_MAPPER_MAXID)
; and assigns bits 8–11 (the high nibble) to @p out_nib.
;
; @param input     Mapper ID (0..4095).
; @param out_nib   Symbol assigned to
;
; @error Emits an error if @p input > 4095.
.macro _convert_mapperid_hi input, out_nib
    .if   input >= 0 && input <= ::_NSK_HEADER_MAPPER_MAXID
        out_nib = (input >> 8) & $0f
    .else
        .error "Invalid value: NSK_HEADER_MAPPER_ID"
    .endif
.endmacro

; @macro _convert_submapperid
; @brief Normalize a NES 2.0 submapper ID.
;
; Validates the submapper ID range (0..@ref _NSK_HEADER_SUBMAPPER_MAXID)
; and assigns the value directly to @p out_nib.
;
; @param    input     Submapper ID 
; @param    out_nib   Symbol assigned to the validated submapper ID.
;
; @error Emits an error if @p input exceeds max.
.macro _convert_submapperid input, out_nib
    .if input >= 0 && input <= ::_NSK_HEADER_SUBMAPPER_MAXID
        out_nib = input
    .else
        .error "Invalid value: NSK_HEADER_SUBMAPPER_ID"
    .endif
.endmacro

; @macro nsk_header_mapper
; @brief Build the NES 2.0 header byte containing mapper and submapper ID.
;
; Composes and writes the mapper high nibble (bits 8–11) and submapper ID
;
; @param    mapperid     NES 2.0 mapper ID (0..4095)
; @param    submapperid  Submapper ID (0..15).
.macro nsk_header_mapper mapperid, submapperid
    ; locals:
    .local nib_mapper
    .local nib_submapper

    ; set:
    _convert_mapperid_hi    mapperid,       nib_mapper
    _convert_submapperid    submapperid,    nib_submapper

    ; write:
    .byte                        \
        (nib_mapper          ) | \
        (nib_submapper  <<  4)
.endmacro

.endif
