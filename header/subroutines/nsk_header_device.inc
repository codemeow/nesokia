; @file nsk_header_device.inc
; @brief Include module for the default expansion device byte setting
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_DEVICE__
::__NSK_HEADER_DEVICE__ = 1

.linecont +

; @macro _check_ifunknown
; @brief Emit an error if no device converter matched.
;
; @param    found  Flag expected to be 1 when any converter recognized 
;                  the device.
;
; @error Emits an error if the device is not found
.macro _check_ifunknown found
.scope
    check:
        .if found = 0
            .error "Invalid value: NSK_HEADER_EXPANSION_DEVICE"
        .endif
.endscope
.endmacro

; @macro _convert_ifbasic
; @brief Pass through “basic” expansion device values.
;
; Recognizes the generic device codes that require no vendor-specific handling
;
; @param    device       Input device symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifbasic device, out_nibits, out_found
.scope
    convert:
        .if                                      \
            device = nsk::device::unspecified || \
            device = nsk::device::multicart

            out_nibits = device
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_iffamicom
; @brief Pass through Famicom-family expansion devices (EXP-port / FC-specific).
;
; Matches Famicom-oriented devices and aliases (controllers, Zappers, 3D System,
; test controller, Four Players simple adapter, Data Recorder bundles, ROB).
;
; @param    device       Input device symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_iffamicom device, out_nibits, out_found
.scope
    convert:
        .if                                                                \
            device = nsk::device::famicom::controller                   || \
            device = nsk::device::famicom::controller_inv               || \
            device = nsk::device::famicom::controller_test              || \
            device = nsk::device::famicom::fourplayers_simple           || \
            device = nsk::device::famicom::keyboardsilent_datarecorder  || \
            device = nsk::device::famicom::keyboardBASIC_datarecorder   || \
            device = nsk::device::famicom::network_controller           || \
            device = nsk::device::famicom::ROB_gyro                     || \
            device = nsk::device::famicom::ROB_stackup                  || \
            device = nsk::device::famicom::zapper_4016                  || \
            device = nsk::device::famicom::zapper_4017                  || \
            device = nsk::device::famicom::zapper_dual                  || \
            device = nsk::device::famicom::3dsystem

            out_nibits = device
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_ifNES
; @brief Pass through NES-branded devices (Four Score/Satellite, Power Pad, etc)
;
; @param    device       Input device symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifNES device, out_nibits, out_found
.scope
    convert:
        .if                                                \
            device = nsk::device::NES::fourscore        || \
            device = nsk::device::NES::powerpad_sidea   || \
            device = nsk::device::NES::powerpad_sideb   || \
            device = nsk::device::NES::vaus

            out_nibits = device
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_ifVS
; @brief Pass through Vs. System devices (pads via $4016/$4017, Vs. Zapper).
;
; @param    device       Input device symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifVS device, out_nibits, out_found
.scope
    convert:
        .if                                            \
            device = nsk::device::VS::system_4016   || \
            device = nsk::device::VS::system_4017   || \
            device = nsk::device::VS::zapper

            out_nibits = device
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_ifbandai
; @brief Pass through Bandai devices (Family Trainer, Oeka Tablet, PnP playsets)
;
; @param    device       Input device symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifbandai device, out_nibits, out_found
.scope
    convert:
        .if                                                        \
            device = nsk::device::bandai::cookingplayset        || \
            device = nsk::device::bandai::familytrainer_sidea   || \
            device = nsk::device::bandai::familytrainer_sideb   || \
            device = nsk::device::bandai::gamepad               || \
            device = nsk::device::bandai::hypershot             || \
            device = nsk::device::bandai::tablet

            out_nibits = device
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_ifkonami
; @brief Pass through Konami devices (Hyper Shot, Exciting Boxing bag, 
; DoReMiKko KB).
;
; @param    device       Input device symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifkonami device, out_nibits, out_found
.scope
    convert:
        .if                                                \
            device = nsk::device::konami::doremikko     || \
            device = nsk::device::konami::hypershot     || \
            device = nsk::device::konami::punchingbag

            out_nibits = device
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_ifIGS
; @brief Pass through IGS devices (Battle Box EEPROM, Pokkun Moguraa).
;
; @param    device       Input device symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifIGS device, out_nibits, out_found
.scope
    convert:
        .if                                            \
            device = nsk::device::IGS::battlebox    || \
            device = nsk::device::IGS::pokkunmogura

            out_nibits = device
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_ifSNES
; @brief Pass through SNES-family input over adapters (two pads, mouse).
;
; @param    device       Input device symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifSNES device, out_nibits, out_found
.scope
    convert:
        .if                                            \
            device = nsk::device::SNES::controllers || \
            device = nsk::device::SNES::mouse_4016  || \
            device = nsk::device::SNES::mouse_4017

            out_nibits = device
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_iftaito
; @brief Pass through Taito “Vaus” (Arkanoid) paddles (NES/FC, prototype, with 
; Data Recorder bundle).
;
; @param    device       Input device symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_iftaito device, out_nibits, out_found
.scope
    convert:
        .if                                                        \
            device = nsk::device::taito::vaus                   || \
            device = nsk::device::taito::vaus_proto             || \
            device = nsk::device::taito::vausdual_datarecorder

            out_nibits = device
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_ifclonekbm
; @brief Pass through assorted third-party / clone / OEM devices 
; (non-keyboard/mouse group)
;
; @param    device       Input device symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifclonekbm device, out_nibits, out_found
.scope
    convert:
        .if                                                        \
            device = nsk::device::abl_pinball                   || \
            device = nsk::device::advancebright_croakykaraoke   || \
            device = nsk::device::ascii_turbofile               || \
            device = nsk::device::broderbund_uforce             || \
            device = nsk::device::capcom_mahjong                || \
            device = nsk::device::coconuts_pachinko             || \
            device = nsk::device::elecom_mahjong                || \
            device = nsk::device::epoch_barcodebattler2         || \
            device = nsk::device::excalibur_sudoku              || \
            device = nsk::device::gigggle_pump                  || \
            device = nsk::device::hummer_fishingrod             || \
            device = nsk::device::IBM_keyboard                  || \
            device = nsk::device::LG_tvremote                   || \
            device = nsk::device::majesco_casinocontroller      || \
            device = nsk::device::nichibutsu_double_fourscore   || \
            device = nsk::device::racermate_bicycle             || \
            device = nsk::device::sharp_cassette                || \
            device = nsk::device::timetop_lightgun              || \
            device = nsk::device::toolworks_miraclepiano        || \
            device = nsk::device::touchgameplayer_mahjong       || \
            device = nsk::device::varie_toprider                || \
            device = nsk::device::venom_dancemat                || \
            device = nsk::device::yonezawa_partytap

            out_nibits = device
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_ifother
; @brief Pass through OEM/clone keyboards and mice.
;
; Includes Bit Corp./Puze Bit-79, BBK/Bubugao, Dongda, Keda, Kewang, UMC 
; (UM6578), Yuxing, Zecheng, and multiple Subor KB/mouse wiring variants
;
; @param    device       Input device symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifother device, out_nibits, out_found
.scope
    convert:
        .if                                                                     \
            device = nsk::device::clonekbm::bitcorp_keyboard                 || \
            device = nsk::device::clonekbm::bubugao_keyboard_mouse_4017      || \
            device = nsk::device::clonekbm::dongda_keyboard                  || \
            device = nsk::device::clonekbm::keda_keyboard                    || \
            device = nsk::device::clonekbm::kewang_keyboard                  || \
            device = nsk::device::clonekbm::UMC_keyboard_mouse_4017          || \
            device = nsk::device::clonekbm::UMC_mouse                        || \
            device = nsk::device::clonekbm::yuxing_mouse_4016                || \
            device = nsk::device::clonekbm::zecheng_keyboard                 || \
            device = nsk::device::clonekbm::SUBOR::keyboard                  || \
            device = nsk::device::clonekbm::SUBOR::keyboard_mouse            || \
            device = nsk::device::clonekbm::SUBOR::keyboard_mouse_4016       || \
            device = nsk::device::clonekbm::SUBOR::keyboard_mouse_4017       || \
            device = nsk::device::clonekbm::SUBOR::keyboard_mousePS2_4017    || \
            device = nsk::device::clonekbm::SUBOR::keyboard_mousePS2inv_4017 || \
            device = nsk::device::clonekbm::SUBOR::keyboard_mouseyux_4016

            out_nibits = device
            out_found .set 1
        .endif
.endscope
.endmacro

; @macro _convert_device
; @brief Resolve a device symbol into the final 8-bit header value.
;
; Tries all vendor/family-specific converters. If none match, emits an error
;
; @param    device       Input device symbol (one of nsk::device::*).
; @param    out_nibits   Symbol assigned to the resolved 8-bit device code.
.macro _convert_device device, out_nibits
.scope
    locals:
        _detected .set 0

    convert:
        _convert_ifbasic        device, out_nibits, _detected
        _convert_iffamicom      device, out_nibits, _detected
        _convert_ifNES          device, out_nibits, _detected
        _convert_ifVS           device, out_nibits, _detected
        _convert_ifbandai       device, out_nibits, _detected
        _convert_ifkonami       device, out_nibits, _detected
        _convert_ifIGS          device, out_nibits, _detected
        _convert_ifSNES         device, out_nibits, _detected
        _convert_iftaito        device, out_nibits, _detected
        _convert_ifclonekbm     device, out_nibits, _detected
        _convert_ifother        device, out_nibits, _detected

    check:
        _check_ifunknown _detected
.endscope
.endmacro

; @macro nsk_header_device
; @brief Write the “Default Expansion Device” byte for the NES 2.0 header.
;
; @param    device  Input device symbol (nsk::device::xxx).
.macro nsk_header_device device
.scope
    locals:
        .local nibits_device

    convert:
        _convert_device device, nibits_device

    write:
        .byte nibits_device
.endscope
.endmacro

.delmacro _convert_device
.delmacro _check_ifunknown
.delmacro _convert_ifbasic
.delmacro _convert_iffamicom
.delmacro _convert_ifNES
.delmacro _convert_ifVS
.delmacro _convert_ifbandai
.delmacro _convert_ifkonami
.delmacro _convert_ifIGS
.delmacro _convert_ifSNES
.delmacro _convert_iftaito
.delmacro _convert_ifclonekbm
.delmacro _convert_ifother

.endif
