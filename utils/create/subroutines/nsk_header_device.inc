; @file nsk_header_deviceid.inc
; @brief Include module for the default expansion deviceid byte setting
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_DEVICE__
::__NSK_HEADER_DEVICE__ = 1

.linecont +

; @macro _check_ifunknown_device
; @brief Emit an error if no deviceid converter matched.
;
; @param    found  Flag expected to be 1 when any converter recognized 
;                  the deviceid.
;
; @error Emits an error if the deviceid is not found
.macro _check_ifunknown_device found
    .if found = 0
        .error "Invalid value: NSK_HEADER_EXPANSION_DEVICE"
    .endif
.endmacro

; @macro _convert_ifbasic
; @brief Pass through “basic” expansion deviceid values.
;
; Recognizes the generic deviceid codes that require no vendor-specific handling
;
; @param    deviceid     Input deviceid symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifbasic deviceid, out_nibits, out_found
    .if                                        \
        deviceid = nsk::device::unspecified || \
        deviceid = nsk::device::multicart

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifnintendo
; @brief Pass through Famicom/NES-family expansion devices
;
; Matches Famicom-oriented deviceids and aliases (controllers, Zappers, 3D System,
; test controller, Four Players simple adapter, Data Recorder bundles, ROB).
;
; @param    deviceid     Input deviceid symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifnintendo deviceid, out_nibits, out_found
    .if                                                                  \
        deviceid = nsk::device::nintendo::controller                   || \
        deviceid = nsk::device::nintendo::controller_inv               || \
        deviceid = nsk::device::nintendo::controller_test              || \
        deviceid = nsk::device::nintendo::fourplayers_simple           || \
        deviceid = nsk::device::nintendo::keyboardsilent_datarecorder  || \
        deviceid = nsk::device::nintendo::keyboardBASIC_datarecorder   || \
        deviceid = nsk::device::nintendo::network_controller           || \
        deviceid = nsk::device::nintendo::ROB_gyro                     || \
        deviceid = nsk::device::nintendo::ROB_stackup                  || \
        deviceid = nsk::device::nintendo::zapper_4016                  || \
        deviceid = nsk::device::nintendo::zapper_4017                  || \
        deviceid = nsk::device::nintendo::zapper_dual                  || \
        deviceid = nsk::device::nintendo::system3d                     || \
        deviceid = nsk::device::nintendo::fourscore                    || \
        deviceid = nsk::device::nintendo::powerpad_sidea               || \
        deviceid = nsk::device::nintendo::powerpad_sideb               || \
        deviceid = nsk::device::nintendo::vaus

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifVS
; @brief Pass through Vs. System devices (pads via $4016/$4017, Vs. Zapper).
;
; @param    deviceid     Input deviceid symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifVS deviceid, out_nibits, out_found
    .if                                              \
        deviceid = nsk::device::VS::system_4016   || \
        deviceid = nsk::device::VS::system_4017   || \
        deviceid = nsk::device::VS::zapper

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifbandai
; @brief Pass through Bandai devices (Family Trainer, Oeka Tablet, PnP playsets)
;
; @param    deviceid     Input deviceid symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifbandai deviceid, out_nibits, out_found
    .if                                                          \
        deviceid = nsk::device::bandai::cookingplayset        || \
        deviceid = nsk::device::bandai::familytrainer_sidea   || \
        deviceid = nsk::device::bandai::familytrainer_sideb   || \
        deviceid = nsk::device::bandai::gamepad               || \
        deviceid = nsk::device::bandai::hypershot             || \
        deviceid = nsk::device::bandai::tablet

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifkonami
; @brief Pass through Konami devices (Hyper Shot, Exciting Boxing bag,
; DoReMiKko KB).
;
; @param    deviceid     Input deviceid symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifkonami deviceid, out_nibits, out_found
    .if                                                  \
        deviceid = nsk::device::konami::doremikko     || \
        deviceid = nsk::device::konami::hypershot     || \
        deviceid = nsk::device::konami::punchingbag

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifIGS
; @brief Pass through IGS devices (Battle Box EEPROM, Pokkun Moguraa).
;
; @param    deviceid     Input deviceid symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifIGS deviceid, out_nibits, out_found
    .if                                              \
        deviceid = nsk::device::IGS::battlebox    || \
        deviceid = nsk::device::IGS::pokkunmogura

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifSNES
; @brief Pass through SNES-family input over adapters (two pads, mouse).
;
; @param    deviceid     Input deviceid symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifSNES deviceid, out_nibits, out_found
    .if                                              \
        deviceid = nsk::device::SNES::controllers || \
        deviceid = nsk::device::SNES::mouse_4016  || \
        deviceid = nsk::device::SNES::mouse_4017

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_iftaito
; @brief Pass through Taito “Vaus” (Arkanoid) paddles (NES/FC, prototype, with 
; Data Recorder bundle).
;
; @param    deviceid     Input deviceid symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_iftaito deviceid, out_nibits, out_found
    .if                                                          \
        deviceid = nsk::device::taito::vaus                   || \
        deviceid = nsk::device::taito::vaus_proto             || \
        deviceid = nsk::device::taito::vausdual_datarecorder

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifclonekbm
; @brief Pass through assorted third-party / clone / OEM devices
; (non-keyboard/mouse group)
;
; @param    deviceid     Input deviceid symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifclonekbm deviceid, out_nibits, out_found
    .if                                                          \
        deviceid = nsk::device::abl_pinball                   || \
        deviceid = nsk::device::advancebright_croakykaraoke   || \
        deviceid = nsk::device::ascii_turbofile               || \
        deviceid = nsk::device::broderbund_uforce             || \
        deviceid = nsk::device::capcom_mahjong                || \
        deviceid = nsk::device::coconuts_pachinko             || \
        deviceid = nsk::device::elecom_mahjong                || \
        deviceid = nsk::device::epoch_barcodebattler2         || \
        deviceid = nsk::device::excalibur_sudoku              || \
        deviceid = nsk::device::gigggle_pump                  || \
        deviceid = nsk::device::hummer_fishingrod             || \
        deviceid = nsk::device::IBM_keyboard                  || \
        deviceid = nsk::device::LG_tvremote                   || \
        deviceid = nsk::device::majesco_casinocontroller      || \
        deviceid = nsk::device::nichibutsu_double_fourscore   || \
        deviceid = nsk::device::racermate_bicycle             || \
        deviceid = nsk::device::sharp_cassette                || \
        deviceid = nsk::device::timetop_lightgun              || \
        deviceid = nsk::device::toolworks_miraclepiano        || \
        deviceid = nsk::device::touchgameplayer_mahjong       || \
        deviceid = nsk::device::varie_toprider                || \
        deviceid = nsk::device::venom_dancemat                || \
        deviceid = nsk::device::yonezawa_partytap

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifother
; @brief Pass through OEM/clone keyboards and mice.
;
; Includes Bit Corp./Puze Bit-79, BBK/Bubugao, Dongda, Keda, Kewang, UMC 
; (UM6578), Yuxing, Zecheng, and multiple Subor KB/mouse wiring variants
;
; @param    deviceid     Input deviceid symbol (nsk::device::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifother deviceid, out_nibits, out_found
    .if                                                                       \
        deviceid = nsk::device::clonekbm::bitcorp_keyboard                 || \
        deviceid = nsk::device::clonekbm::bubugao_keyboard_mouse_4017      || \
        deviceid = nsk::device::clonekbm::dongda_keyboard                  || \
        deviceid = nsk::device::clonekbm::keda_keyboard                    || \
        deviceid = nsk::device::clonekbm::kewang_keyboard                  || \
        deviceid = nsk::device::clonekbm::UMC_keyboard_mouse_4017          || \
        deviceid = nsk::device::clonekbm::UMC_mouse                        || \
        deviceid = nsk::device::clonekbm::yuxing_mouse_4016                || \
        deviceid = nsk::device::clonekbm::zecheng_keyboard                 || \
        deviceid = nsk::device::clonekbm::SUBOR::keyboard                  || \
        deviceid = nsk::device::clonekbm::SUBOR::keyboard_mouse            || \
        deviceid = nsk::device::clonekbm::SUBOR::keyboard_mouse_4016       || \
        deviceid = nsk::device::clonekbm::SUBOR::keyboard_mouse_4017       || \
        deviceid = nsk::device::clonekbm::SUBOR::keyboard_mousePS2_4017    || \
        deviceid = nsk::device::clonekbm::SUBOR::keyboard_mousePS2inv_4017 || \
        deviceid = nsk::device::clonekbm::SUBOR::keyboard_mouseyux_4016

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_deviceid
; @brief Resolve a deviceid symbol into the final 8-bit header value.
;
; Tries all vendor/family-specific converters. If none match, emits an error
;
; @param    deviceid     Input deviceid symbol (one of nsk::device::*).
; @param    out_nibits   Symbol assigned to the resolved 8-bit deviceid code.
.macro _convert_deviceid deviceid, out_nibits
    ; locals:
    _detected .set 0

    ; convert:
    _convert_ifbasic        deviceid, out_nibits, _detected
    _convert_ifnintendo     deviceid, out_nibits, _detected
    _convert_ifVS           deviceid, out_nibits, _detected
    _convert_ifbandai       deviceid, out_nibits, _detected
    _convert_ifkonami       deviceid, out_nibits, _detected
    _convert_ifIGS          deviceid, out_nibits, _detected
    _convert_ifSNES         deviceid, out_nibits, _detected
    _convert_iftaito        deviceid, out_nibits, _detected
    _convert_ifclonekbm     deviceid, out_nibits, _detected
    _convert_ifother        deviceid, out_nibits, _detected

    ; check:
    _check_ifunknown_device _detected
.endmacro

; @macro nsk_header_deviceid
; @brief Write the “Default Expansion Device” byte for the NES 2.0 header.
;
; @param    deviceid  Input deviceid symbol (nsk::device::xxx).
.macro nsk_header_device deviceid
    ; locals:
    .local nibits_deviceid

    ; convert:
    _convert_deviceid deviceid, nibits_deviceid

    ; write:
    .byte nibits_deviceid
.endmacro

.endif
