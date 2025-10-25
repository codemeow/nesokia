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
; @param    region   Region selector (`nsk::region::xxx`).
; @param    console  Console selector (`nsk::console::xxx`).
;
; @error Emits a error if @p console is one of the VTxx values.
.macro _validate_vrtech region, console
.scope
    validate:
        .if                                                \
            console = nsk::console::VR_Technology_VT01  || \
            console = nsk::console::VR_Technology_VT02  || \
            console = nsk::console::VR_Technology_VT03  || \
            console = nsk::console::VR_Technology_VT09  || \
            console = nsk::console::VR_Technology_VT32  || \
            console = nsk::console::VR_Technology_VT369

            .error                                                  \
                .sprintf(                                           \
                    "V.R. Technology boards cannot have %d region", \
                    region                                          \
                )
        .endif
.endscope
.endmacro

; @macro _convert_ifntsc
; @brief If @p region is any NTSC alias, pass it through and mark as found.
;
; @param    region     Region selector (`nsk::region::xxx`).
; @param    console    Console selector (`nsk::console::xxx`)
; @param    out_bits   Symbol assigned to the resulting region byte.
; @param    out_found  Symbol set to 1 if a match was found (otherwise unchanged).
.macro _convert_ifntsc region, console, out_bits, out_found
.scope
    convert:
        .if                                            \
            region = nsk::region::RP2C02            || \
            region = nsk::region::NTSC              || \
            region = nsk::region::North_America     || \
            region = nsk::region::Japan             || \
            region = nsk::region::South_Korea       || \
            region = nsk::region::Taiwan

            out_bits = region
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_ifpal
; @brief If @p region is any PAL alias, pass it through and mark as found.
;
; @param    region     Region selector (`nsk::region::xxx`).
; @param    console    Console selector (`nsk::console::xxx`)
; @param    out_bits   Symbol assigned to the resulting region byte.
; @param    out_found  Symbol set to 1 if a match was found (otherwise unchanged).
.macro _convert_ifpal region, console, out_bits, out_found
.scope
    validate:
        _validate_vrtech region, console

    convert:
        .if                                            \
            region = nsk::region::RP2C07            || \
            region = nsk::region::PAL               || \
            region = nsk::region::Western_Europe    || \
            region = nsk::region::Australia

            out_bits = region
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_ifntsc
; @brief If @p region is `Multiple` value, pass it through and mark as found.
;
; @param    region     Region selector (`nsk::region::xxx`).
; @param    console    Console selector (`nsk::console::xxx`)
; @param    out_bits   Symbol assigned to the resulting region byte.
; @param    out_found  Symbol set to 1 if a match was found (otherwise unchanged).
.macro _convert_ifmultiple region, console, out_bits, out_found
.scope
    validate:
        _validate_vrtech region, console

    convert:
        .if                                            \
            region = nsk::region::Multiple

            out_bits = region
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_ifntsc
; @brief If @p region is any Dendy alias, pass it through and mark as found.
;
; @param    region     Region selector (`nsk::region::xxx`).
; @param    console    Console selector (`nsk::console::xxx`)
; @param    out_bits   Symbol assigned to the resulting region byte.
; @param    out_found  Symbol set to 1 if a match was found (otherwise unchanged).
.macro _convert_ifdendy region, console, out_bits, out_found
.scope
    convert:
        .if                                            \
            region = nsk::region::UA6538            || \
            region = nsk::region::Dendy             || \
            region = nsk::region::Eastern_Europe    || \
            region = nsk::region::Russia            || \
            region = nsk::region::Mainland_China    || \
            region = nsk::region::India             || \
            region = nsk::region::Africa

            out_bits = region
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _check_ifunknown
; @brief Emit an error if no region converter matched.
;
; @param    region  Region value that failed to match.
; @param    found   Flag expected to be 1 when a converter matched
;
; @error Emits an error with the provided @p region value.
.macro _check_ifunknown region, found
.scope
    check:
        .if found = 0
            .error                                  \
                .sprintf(                           \
                    "Unknown console region: %d",   \
                    region                          \
                )
        .endif
.endscope
.endmacro

; @macro _check_vssystem
; @brief Checks if the VS system console has th valid mapper selection
;
; @param    console  Console selector (`nsk::console::xxx` values).
; @param    mapperid Mapper ID
.macro _check_vssystem console, mapperid
.scope
    check:
        .if console = nsk::console::nintendo_vs_system
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
                        region                                                     \
                    )
            .endif
        .endif
.endscope
.endmacro

; @macro nsk_header_timings
; @brief Resolve and write the region/timing byte for the NES 2.0 header.
;
; Tries NTSC, PAL, Multiple, and Dendy-class region.
; When a match is found, writes the corresponding region code as a single byte.
;
; @param    region   Region selector (`nsk::region::xxx` values).
; @param    console  Console selector (`nsk::console::xxx` values).
.macro nsk_header_timings region, console, mapperid
.scope
    locals:
        .local bits_region
        _detected .set 0

    validate:
        _check_vssystem     console, mapperid

    convert:
        _convert_ifntsc     region, console, bits_region, _detected
        _convert_ifpal      region, console, bits_region, _detected
        _convert_ifmultiple region, console, bits_region, _detected
        _convert_ifdendy    region, console, bits_region, _detected

    check:
        _check_ifunknown    region, _detected

    write:
        .byte bits_region
.endscope
.endmacro

.delmacro _validate_vrtech
.delmacro _convert_ifntsc
.delmacro _convert_ifpal
.delmacro _convert_ifmultiple
.delmacro _convert_ifdendy
.delmacro _check_ifunknown
.delmacro _check_vssystem

.endif
