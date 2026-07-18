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
; @param    deviceid Input deviceid symbol
; @param    found    Flag expected to be 1 when any converter recognized
;                    the deviceid.
;
; @error Emits an error if the deviceid is not found
.macro _check_ifunknown_device deviceid, found
    .if found = 0
        .error .sprintf("Invalid value: NSK_HEADER_EXPANSION_DEVICE: %d", deviceid)
    .endif
.endmacro

; @macro _convert_ifbasic
; @brief Pass through “basic” expansion deviceid values.
;
; Recognizes the generic deviceid codes that require no vendor-specific handling
;
; @param    deviceid     Input deviceid symbol (NSKHW::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifbasic deviceid, out_nibits, out_found
    .if                                               \
        deviceid = NSKHW::DEVICES::NONE::UNSPECIFIED || \
        deviceid = NSKHW::DEVICES::NONE::MULTICART

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
; @param    deviceid     Input deviceid symbol (NSKHW::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifnintendo deviceid, out_nibits, out_found
    .if                                                                    \
        deviceid = NSKHW::DEVICES::NINTENDO::CONTROLLERS                  || \
        deviceid = NSKHW::DEVICES::NINTENDO::CONTROLLERS_INV              || \
        deviceid = NSKHW::DEVICES::NINTENDO::CONTROLLERS_TEST             || \
        deviceid = NSKHW::DEVICES::NINTENDO::FOURPLAYERS_SIMPLE           || \
        deviceid = NSKHW::DEVICES::NINTENDO::KEYBOARD_NOINPUT_DATAREC     || \
        deviceid = NSKHW::DEVICES::NINTENDO::KEYBOARD_BASIC_DATAREC       || \
        deviceid = NSKHW::DEVICES::NINTENDO::CONTROLLER_NETWORK           || \
        deviceid = NSKHW::DEVICES::NINTENDO::ROB_GYRO                     || \
        deviceid = NSKHW::DEVICES::NINTENDO::ROB_STACKUP                  || \
        deviceid = NSKHW::DEVICES::NINTENDO::ZAPPER_P4016                 || \
        deviceid = NSKHW::DEVICES::NINTENDO::ZAPPER_P4017                 || \
        deviceid = NSKHW::DEVICES::NINTENDO::ZAPPER_DUAL                  || \
        deviceid = NSKHW::DEVICES::NINTENDO::SYSTEM3D                     || \
        deviceid = NSKHW::DEVICES::NINTENDO::FOURSCORE                    || \
        deviceid = NSKHW::DEVICES::NINTENDO::POWERPAD_A                   || \
        deviceid = NSKHW::DEVICES::NINTENDO::POWERPAD_B                   || \
        deviceid = NSKHW::DEVICES::NINTENDO::VAUS

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifVS
; @brief Pass through Vs. System devices (pads via $4016/$4017, Vs. Zapper).
;
; @param    deviceid     Input deviceid symbol (NSKHW::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifVS deviceid, out_nibits, out_found
    .if                                              \
        deviceid = NSKHW::DEVICES::VSSYSTEM::P4016  || \
        deviceid = NSKHW::DEVICES::VSSYSTEM::P4017  || \
        deviceid = NSKHW::DEVICES::VSSYSTEM::ZAPPER

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifbandai
; @brief Pass through Bandai devices (Family Trainer, Oeka Tablet, PnP playsets)
;
; @param    deviceid     Input deviceid symbol (NSKHW::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifbandai deviceid, out_nibits, out_found
    .if                                                           \
        deviceid = NSKHW::DEVICES::BANDAI::COOKINGPLAYSET        || \
        deviceid = NSKHW::DEVICES::BANDAI::FAMILYTRAINER_A       || \
        deviceid = NSKHW::DEVICES::BANDAI::FAMILYTRAINER_B       || \
        deviceid = NSKHW::DEVICES::BANDAI::GAMEPAD               || \
        deviceid = NSKHW::DEVICES::BANDAI::HYPERSHOT             || \
        deviceid = NSKHW::DEVICES::BANDAI::TABLET

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifkonami
; @brief Pass through Konami devices (Hyper Shot, Exciting Boxing bag,
; DoReMiKko KB).
;
; @param    deviceid     Input deviceid symbol (NSKHW::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifkonami deviceid, out_nibits, out_found
    .if                                                   \
        deviceid = NSKHW::DEVICES::KONAMI::DOREMIKKO     || \
        deviceid = NSKHW::DEVICES::KONAMI::HYPERSHOT     || \
        deviceid = NSKHW::DEVICES::KONAMI::PUNCHINGBAG

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifIGS
; @brief Pass through IGS devices (Battle Box EEPROM, Pokkun Moguraa).
;
; @param    deviceid     Input deviceid symbol (NSKHW::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifIGS deviceid, out_nibits, out_found
    .if                                               \
        deviceid = NSKHW::DEVICES::IGS::BATTLEBOX    || \
        deviceid = NSKHW::DEVICES::IGS::POKKUNMOGURA

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifSNES
; @brief Pass through SNES-family input over adapters (two pads, mouse).
;
; @param    deviceid     Input deviceid symbol (NSKHW::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifSNES deviceid, out_nibits, out_found
    .if                                               \
        deviceid = NSKHW::DEVICES::SNES::CONTROLLERS || \
        deviceid = NSKHW::DEVICES::SNES::MOUSE_P4016 || \
        deviceid = NSKHW::DEVICES::SNES::MOUSE_P4017

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_iftaito
; @brief Pass through Taito “Vaus” (Arkanoid) paddles (NES/FC, prototype, with 
; Data Recorder bundle).
;
; @param    deviceid     Input deviceid symbol (NSKHW::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_iftaito deviceid, out_nibits, out_found
    .if                                                           \
        deviceid = NSKHW::DEVICES::TAITO::VAUS                   || \
        deviceid = NSKHW::DEVICES::TAITO::VAUS_PROTO             || \
        deviceid = NSKHW::DEVICES::TAITO::VAUS_DUAL_DATAREC

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifclonekbm
; @brief Pass through assorted third-party / clone / OEM devices
; (non-keyboard/mouse group)
;
; @param    deviceid     Input deviceid symbol (NSKHW::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifclonekbm deviceid, out_nibits, out_found
    .if                                                            \
        deviceid = NSKHW::DEVICES::ABL::PINBALL                   || \
        deviceid = NSKHW::DEVICES::ADVANCEDBRIGHT::CROAKYKARAOKE  || \
        deviceid = NSKHW::DEVICES::ASCII::TURBOFILE               || \
        deviceid = NSKHW::DEVICES::BRODERBUND::UFORCE             || \
        deviceid = NSKHW::DEVICES::CAPCOM::MAHJONG                || \
        deviceid = NSKHW::DEVICES::COCONUTS::PACHINKO             || \
        deviceid = NSKHW::DEVICES::ELECOM::MAHJONG                || \
        deviceid = NSKHW::DEVICES::EPOCH::BARCODEBATTLER2         || \
        deviceid = NSKHW::DEVICES::EXCALIBUR::SUDOKU              || \
        deviceid = NSKHW::DEVICES::GIGGGLE::PUMP                  || \
        deviceid = NSKHW::DEVICES::HUMMER::FISHINGROD             || \
        deviceid = NSKHW::DEVICES::IBM::KEYBOARD                  || \
        deviceid = NSKHW::DEVICES::LG::TVREMOTE                   || \
        deviceid = NSKHW::DEVICES::MAJESCO::CASINOCONTROLLER      || \
        deviceid = NSKHW::DEVICES::NICHIBUTSU::DOUBLE             || \
        deviceid = NSKHW::DEVICES::RACERMATE::BICYCLE             || \
        deviceid = NSKHW::DEVICES::SHARP::CASSETTE                || \
        deviceid = NSKHW::DEVICES::TIMETOP::LIGHTGUN              || \
        deviceid = NSKHW::DEVICES::TOOLWORKS::MIRACLEPIANO        || \
        deviceid = NSKHW::DEVICES::TOUCHGAMEPLAYER::MAHJONG       || \
        deviceid = NSKHW::DEVICES::VARIE::TOPRIDER                || \
        deviceid = NSKHW::DEVICES::VENOM::DANCEMAT                || \
        deviceid = NSKHW::DEVICES::YONEZAWA::PARTYTAP

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
; @param    deviceid     Input deviceid symbol (NSKHW::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifother deviceid, out_nibits, out_found
    .if                                                               \
        deviceid = NSKHW::DEVICES::BITCORP::KEYBOARD                 || \
        deviceid = NSKHW::DEVICES::BUBUGAO::KEYBOARD_MOUSE_P4017     || \
        deviceid = NSKHW::DEVICES::DONGDA::KEYBOARD                  || \
        deviceid = NSKHW::DEVICES::KEDA::KEYBOARD                    || \
        deviceid = NSKHW::DEVICES::KEWANG::KEYBOARD                  || \
        deviceid = NSKHW::DEVICES::UMC::KEYBOARD_MOUSE_P4017         || \
        deviceid = NSKHW::DEVICES::UMC::MOUSE                        || \
        deviceid = NSKHW::DEVICES::YUXING::MOUSE_P4016               || \
        deviceid = NSKHW::DEVICES::ZECHENG::KEYBOARD                 || \
        deviceid = NSKHW::DEVICES::SUBOR::KEYBOARD                   || \
        deviceid = NSKHW::DEVICES::SUBOR::KEYBOARD_MOUSE             || \
        deviceid = NSKHW::DEVICES::SUBOR::KEYBOARD_MOUSE_P4016       || \
        deviceid = NSKHW::DEVICES::SUBOR::KEYBOARD_MOUSE_P4017       || \
        deviceid = NSKHW::DEVICES::SUBOR::KEYBOARD_MOUSEPS2_P4017    || \
        deviceid = NSKHW::DEVICES::SUBOR::KEYBOARD_MOUSEPS2INV_P4017 || \
        deviceid = NSKHW::DEVICES::SUBOR::KEYBOARD_MOUSEYUX_P4016

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_deviceid
; @brief Resolve a deviceid symbol into the final 8-bit header value.
;
; Tries all vendor/family-specific converters. If none match, emits an error
;
; @param    deviceid     Input deviceid symbol (one of NSKHW::DEVICES::*).
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
    _check_ifunknown_device deviceid, _detected
.endmacro

; @macro nsk_header_deviceid
; @brief Write the “Default Expansion Device” byte for the NES 2.0 header.
;
; @param    deviceid  Input deviceid symbol (NSKHW::DEVICES::xxx).
.macro nsk_header_device deviceid
    ; locals:
    .local nibits_deviceid

    ; convert:
    _convert_deviceid deviceid, nibits_deviceid

    ; write:
    .byte nibits_deviceid
.endmacro

.endif
