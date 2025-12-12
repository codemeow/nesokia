; @file nsk_header_timings.inc
; @brief Include module for the console region encoding
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_TIMINGS__
::__NSK_HEADER_TIMINGS__ = 1

.linecont +

; @macro _validate_vrtech
; @brief Reject region assignments for V.R. Technology (VTxx) console variants.
;
; Ensures that VT01/VT02/VT03/VT09/VT32/VT369 consoles are not paired with an
; explicit region value. These SoC-based clones have fixed timing/behavior
; and should not carry a separate region code.
;
; @param    regionid   Region selector (`nsk::region::xxx`).
; @param    consoleid  Console selector (`nsk::console::xxx`).
;
; @error Emits a error if @p console is one of the VTxx values.
.macro _validate_vrtech regionid, consoleid
    .if                                                 \
        consoleid = NSK::CONSOLE::VRTECHNOLOGY_VT01  || \
        consoleid = NSK::CONSOLE::VRTECHNOLOGY_VT02  || \
        consoleid = NSK::CONSOLE::VRTECHNOLOGY_VT03  || \
        consoleid = NSK::CONSOLE::VRTECHNOLOGY_VT09  || \
        consoleid = NSK::CONSOLE::VRTECHNOLOGY_VT32  || \
        consoleid = NSK::CONSOLE::VRTECHNOLOGY_VT369

        .error                                                  \
            .sprintf(                                           \
                "V.R. Technology boards cannot have %d region", \
                regionid                                        \
            )
    .endif
.endmacro

; @macro _convert_ifntsc
; @brief If @p regionid is any NTSC alias, pass it through and mark as found.
;
; @param    regionid   Region selector (`nsk::region::xxx`).
; @param    consoleid  Console selector (`nsk::console::xxx`)
; @param    out_bits   Symbol assigned to the resulting region byte.
; @param    out_found  Symbol set to 1 if a match was found (otherwise unchanged).
.macro _convert_ifntsc regionid, consoleid, out_bits, out_found
    .if                                              \
        regionid = NSK::REGION::RP2C02            || \
        regionid = NSK::REGION::NTSC              || \
        regionid = NSK::REGION::NORTH_AMERICA     || \
        regionid = NSK::REGION::JAPAN             || \
        regionid = NSK::REGION::SOUTH_KOREA       || \
        regionid = NSK::REGION::TAIWAN

        out_bits = regionid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifpal
; @brief If @p regionid is any PAL alias, pass it through and mark as found.
;
; @param    regionid   Region selector (`nsk::region::xxx`).
; @param    consoleid  Console selector (`nsk::console::xxx`)
; @param    out_bits   Symbol assigned to the resulting region byte.
; @param    out_found  Symbol set to 1 if a match was found (otherwise unchanged).
.macro _convert_ifpal regionid, consoleid, out_bits, out_found
    ; validate:
    _validate_vrtech regionid, consoleid

    ; convert:
    .if                                              \
        regionid = NSK::REGION::RP2C07            || \
        regionid = NSK::REGION::PAL               || \
        regionid = NSK::REGION::WESTERN_EUROPE    || \
        regionid = NSK::REGION::AUSTRALIA

        out_bits = regionid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifntsc
; @brief If @p regionid is `Multiple` value, pass it through and mark as found.
;
; @param    regionid   Region selector (`nsk::region::xxx`).
; @param    consoleid  Console selector (`nsk::console::xxx`)
; @param    out_bits   Symbol assigned to the resulting region byte.
; @param    out_found  Symbol set to 1 if a match was found (otherwise unchanged).
.macro _convert_ifmultiple regionid, consoleid, out_bits, out_found
    ; validate:
    _validate_vrtech regionid, consoleid

    ; convert:
    .if                                            \
        regionid = NSK::REGION::MULTIPLE

        out_bits = regionid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifntsc
; @brief If @p regionid is any Dendy alias, pass it through and mark as found.
;
; @param    regionid   Region selector (`nsk::region::xxx`).
; @param    consoleid  Console selector (`nsk::console::xxx`)
; @param    out_bits   Symbol assigned to the resulting region byte.
; @param    out_found  Symbol set to 1 if a match was found (otherwise unchanged).
.macro _convert_ifdendy regionid, consoleid, out_bits, out_found
    .if                                              \
        regionid = NSK::REGION::UA6538            || \
        regionid = NSK::REGION::DENDY             || \
        regionid = NSK::REGION::EASTERN_EUROPE    || \
        regionid = NSK::REGION::RUSSIA            || \
        regionid = NSK::REGION::MAINLAND_CHINA    || \
        regionid = NSK::REGION::INDIA             || \
        regionid = NSK::REGION::AFRICA

        out_bits = regionid
        out_found .set 1
    .endif
.endmacro

; @macro _check_ifunknown
; @brief Emit an error if no region converter matched.
;
; @param    regionid Region value that failed to match.
; @param    found    Flag expected to be 1 when a converter matched
;
; @error Emits an error with the provided @p regionid value.
.macro _check_ifunknown regionid, out_found
    .if out_found = 0
        .error                                  \
            .sprintf(                           \
                "Unknown console region: %d",   \
                regionid                        \
            )
    .endif
.endmacro

; @macro _check_vssystem
; @brief Checks if the VS system console has th valid mapper selection
;
; @param    consoleid Console selector (`nsk::console::xxx` values).
; @param    mapperid  Mapper ID
.macro _check_vssystem consoleid, mapperid
    .if consoleid = NSK::CONSOLE::NINTENDO_VS
        .if !(                 \
            mapperid =    1 || \
            mapperid =    2 || \
            mapperid =   67 || \
            mapperid =   75 || \
            mapperid =   99 || \
            mapperid =  206    \
        )
            .error                                                             \
                .sprintf(                                                      \
                    "Mapper %d is not an acceptable value for the VS console", \
                    mapperid                                                   \
                )
        .endif
    .endif
.endmacro

; @macro nsk_header_timings
; @brief Resolve and write the region/timing byte for the NES 2.0 header.
;
; Tries NTSC, PAL, Multiple, and Dendy-class region.
; When a match is found, writes the corresponding region code as a single byte.
;
; @param    regionid   Region selector (`nsk::region::xxx` values).
; @param    consoleid  Console selector (`nsk::console::xxx` values).
.macro nsk_header_timings regionid, consoleid, mapperid
    ; locals:
    .local bits_region
    _detected .set 0

    ; validate:
    _check_vssystem     consoleid, mapperid

    ; convert:
    _convert_ifntsc     regionid, consoleid, bits_region, _detected
    _convert_ifpal      regionid, consoleid, bits_region, _detected
    _convert_ifmultiple regionid, consoleid, bits_region, _detected
    _convert_ifdendy    regionid, consoleid, bits_region, _detected

    ; check:
    _check_ifunknown    regionid, _detected

    ; write:
    .byte bits_region
.endmacro

.endif
