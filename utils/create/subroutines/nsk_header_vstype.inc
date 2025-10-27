; @file nsk_header_vstype.inc
; @brief Constant definitions and enumerations for NES 2.0 headers.
;
; Provides named values for flags, and configuration
; options referenced in `nsk_header_config.inc`.
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_VSTYPE__
::__NSK_HEADER_VSTYPE__ = 1

.linecont +

; @macro _check_ifisvs
; @brief Detect whether the console is Vs. System.
;
; Sets @p out_isvs to 1 iff @p console equals @ref console::nintendo_vs_system,
; otherwise sets it to 0.
;
; @param    consoleid Console selector (value from @ref console).
; @param    out_isvs  Symbol assigned to {0,1}.
.macro _check_ifisvs consoleid, out_isvs
    .if consoleid = nsk::console::nintendo_vs_system
        out_isvs = 1

    .else
        out_isvs = 0
     .endif
.endmacro

; @macro _check_ifextended
; @brief Classify console as “extended” (NES 2.0 Console Type = $03) or base.
;
; - For base/licensed families (Famicom/NES/Dendy/Vs./PlayChoice-10) sets @p 
; out_isext=0.
; - For extended families (famiclones, VTxx, UMC, Famicom Network System) 
; sets @p out_isext=1.
;
; @param    consoleid  Console selector (`nsk::console::xxx`).
; @param    out_isext  Symbol assigned to {0,1}.
;
; @error Emits an error if @p consoleid is unknown.
.macro _check_ifextended consoleid, out_isext
    .if                                                      \
        consoleid = nsk::console::famicom                 || \
        consoleid = nsk::console::NES                     || \
        consoleid = nsk::console::dendy                   || \
        consoleid = nsk::console::nintendo_vs_system      || \
        consoleid = nsk::console::nintendo_playchoice_10

        out_isext = 0

    .elseif                                                  \
        consoleid = nsk::console::famiclone_decimal       || \
        consoleid = nsk::console::famicom_epsm            || \
        consoleid = nsk::console::VR_Technology_VT01      || \
        consoleid = nsk::console::VR_Technology_VT02      || \
        consoleid = nsk::console::VR_Technology_VT03      || \
        consoleid = nsk::console::VR_Technology_VT09      || \
        consoleid = nsk::console::VR_Technology_VT32      || \
        consoleid = nsk::console::VR_Technology_VT369     || \
        consoleid = nsk::console::UMC_UM6578              || \
        consoleid = nsk::console::famicom_network_system

        out_isext = 1
    .else

        .error "Invalid value: NSK_HEADER_CONSOLE_TYPE"
    .endif
.endmacro

; @macro _convert_none
; @brief Encode “no Vs./extended type” into output byte (0x00).
;
; @param    out_res  Symbol assigned to $00.
.macro _convert_none out_res
    out_res = $00
.endmacro

; @macro _convert_extended
; @brief Pass through extended console code into output byte.
;
; @param    consoleid Extended console selector (`nsk::console::xxx`).
; @param    out_res   Symbol assigned to the console code.
.macro _convert_extended consoleid, out_res
    out_res = consoleid
.endmacro

; @macro _convert_vssystem
; @brief Pack Vs. System PPU type (low nibble) and hardware type (high nibble).
;
; Validates @p pputype against known Vs. PPU identifiers and @p hardwaretype
; against Vs. Unisystem/DualSystem hardware variants, then composes a byte:
;   bits 0–3 : Vs. PPU type
;   bits 4–7 : Vs. hardware type (header byte 13 D7..D4)
;
; @param    pputype       One of nsk::console::vs::ppu::xxx
; @param    hardwaretype  One of nsk::console::vs::hardware::xxx
; @param    out_res       Symbol assigned to the packed byte.
;
; @error Emits an error if @p pputype is not recognized.
; @error Emits an error if @p hardwaretype is not recognized.
.macro _convert_vssystem pputype, hardwaretype, out_res
    ; locals:
    .local nib_ppu
    .local nib_hardware

    ; ppu:
    .if                                                    \
        pputype = nsk::console::vs::ppu::RP2C03         || \
        pputype = nsk::console::vs::ppu::RC2C03         || \
        pputype = nsk::console::vs::ppu::RP2C04::rev1   || \
        pputype = nsk::console::vs::ppu::RP2C04::rev2   || \
        pputype = nsk::console::vs::ppu::RP2C04::rev3   || \
        pputype = nsk::console::vs::ppu::RP2C04::rev4   || \
        pputype = nsk::console::vs::ppu::RC2C05::rev1   || \
        pputype = nsk::console::vs::ppu::RC2C05::rev2   || \
        pputype = nsk::console::vs::ppu::RC2C05::rev3   || \
        pputype = nsk::console::vs::ppu::RC2C05::rev4

        nib_ppu = pputype
    .else
        .error "Invalid value: NSK_HEADER_VS_PPU"
    .endif

    ; hardware:
    .if                                                                                    \
        hardwaretype = nsk::console::vs::hardware::unisystem::normal                    || \
        hardwaretype = nsk::console::vs::hardware::unisystem::RBI_baseball              || \
        hardwaretype = nsk::console::vs::hardware::unisystem::TKO_boxing                || \
        hardwaretype = nsk::console::vs::hardware::unisystem::super_xevious             || \
        hardwaretype = nsk::console::vs::hardware::unisystem::ice_climber_japan         || \
        hardwaretype = nsk::console::vs::hardware::dualsystem::normal                   || \
        hardwaretype = nsk::console::vs::hardware::dualsystem::raid_on_bungeling_bay

        nib_hardware = hardwaretype
    .else
        .error "Invalid value: NSK_HEADER_VS_HARDWARE"
    .endif

    ; write:
    out_res =                   \
        (nib_ppu            ) | \
        (nib_hardware <<   4)
.endmacro

; @macro nsk_header_vstype
; @brief Emit NES 2.0 header byte for Vs./extended console type selection.
;
; Decision logic:
;   1) If console is base (not extended) and not Vs. → write $00
;   2) If console is extended                        → pass console code
;   3) If console is Vs. System                      → pack PPU+hardware
;
; @param    pputype       Vs. PPU type (`nsk::console::vs::ppu::xxx`)
; @param    hardwaretype  Vs. hardware type (`nsk::console::vs::hardware::xxx`)
; @param    consoleid     Console selector (`nsk::console::xxx`)
.macro nsk_header_vstype pputype, hardwaretype, consoleid
    ; locals:
    .local byte_out
    .local flag_isext
    .local flag_isvs

    ; flags_set:
    _check_ifextended consoleid, flag_isext
    _check_ifisvs     consoleid, flag_isvs

    ; select:
    .if flag_isext = 0 && flag_isvs = 0
        _convert_none byte_out
            
    .elseif flag_isext = 1
        _convert_extended consoleid, byte_out

    .elseif flag_isvs = 1
        _convert_vssystem pputype, hardwaretype, byte_out
            
    .endif

    ; write:
    .byte byte_out
.endmacro

.endif
