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
; @param    deviceid     Input deviceid symbol (NSK::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifbasic deviceid, out_nibits, out_found
    .if                                               \
        deviceid = NSK::DEVICES::NONE::UNSPECIFIED || \
        deviceid = NSK::DEVICES::NONE::MULTICART

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
; @param    deviceid     Input deviceid symbol (NSK::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifnintendo deviceid, out_nibits, out_found
    .if                                                                    \
        deviceid = NSK::DEVICES::NINTENDO::CONTROLLERS                  || \
        deviceid = NSK::DEVICES::NINTENDO::CONTROLLERS_INV              || \
        deviceid = NSK::DEVICES::NINTENDO::CONTROLLERS_TEST             || \
        deviceid = NSK::DEVICES::NINTENDO::FOURPLAYERS_SIMPLE           || \
        deviceid = NSK::DEVICES::NINTENDO::KEYBOARD_NOINPUT_DATAREC     || \
        deviceid = NSK::DEVICES::NINTENDO::KEYBOARD_BASIC_DATAREC       || \
        deviceid = NSK::DEVICES::NINTENDO::CONTROLLER_NETWORK           || \
        deviceid = NSK::DEVICES::NINTENDO::ROB_GYRO                     || \
        deviceid = NSK::DEVICES::NINTENDO::ROB_STACKUP                  || \
        deviceid = NSK::DEVICES::NINTENDO::ZAPPER_P4016                 || \
        deviceid = NSK::DEVICES::NINTENDO::ZAPPER_P4017                 || \
        deviceid = NSK::DEVICES::NINTENDO::ZAPPER_DUAL                  || \
        deviceid = NSK::DEVICES::NINTENDO::SYSTEM3D                     || \
        deviceid = NSK::DEVICES::NINTENDO::FOURSCORE                    || \
        deviceid = NSK::DEVICES::NINTENDO::POWERPAD_A                   || \
        deviceid = NSK::DEVICES::NINTENDO::POWERPAD_B                   || \
        deviceid = NSK::DEVICES::NINTENDO::VAUS

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifVS
; @brief Pass through Vs. System devices (pads via $4016/$4017, Vs. Zapper).
;
; @param    deviceid     Input deviceid symbol (NSK::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifVS deviceid, out_nibits, out_found
    .if                                              \
        deviceid = NSK::DEVICES::VSSYSTEM::P4016  || \
        deviceid = NSK::DEVICES::VSSYSTEM::P4017  || \
        deviceid = NSK::DEVICES::VSSYSTEM::ZAPPER

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifbandai
; @brief Pass through Bandai devices (Family Trainer, Oeka Tablet, PnP playsets)
;
; @param    deviceid     Input deviceid symbol (NSK::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifbandai deviceid, out_nibits, out_found
    .if                                                           \
        deviceid = NSK::DEVICES::BANDAI::COOKINGPLAYSET        || \
        deviceid = NSK::DEVICES::BANDAI::FAMILYTRAINER_A       || \
        deviceid = NSK::DEVICES::BANDAI::FAMILYTRAINER_B       || \
        deviceid = NSK::DEVICES::BANDAI::GAMEPAD               || \
        deviceid = NSK::DEVICES::BANDAI::HYPERSHOT             || \
        deviceid = NSK::DEVICES::BANDAI::TABLET

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifkonami
; @brief Pass through Konami devices (Hyper Shot, Exciting Boxing bag,
; DoReMiKko KB).
;
; @param    deviceid     Input deviceid symbol (NSK::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifkonami deviceid, out_nibits, out_found
    .if                                                   \
        deviceid = NSK::DEVICES::KONAMI::DOREMIKKO     || \
        deviceid = NSK::DEVICES::KONAMI::HYPERSHOT     || \
        deviceid = NSK::DEVICES::KONAMI::PUNCHINGBAG

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifIGS
; @brief Pass through IGS devices (Battle Box EEPROM, Pokkun Moguraa).
;
; @param    deviceid     Input deviceid symbol (NSK::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifIGS deviceid, out_nibits, out_found
    .if                                               \
        deviceid = NSK::DEVICES::IGS::BATTLEBOX    || \
        deviceid = NSK::DEVICES::IGS::POKKUNMOGURA

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifSNES
; @brief Pass through SNES-family input over adapters (two pads, mouse).
;
; @param    deviceid     Input deviceid symbol (NSK::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifSNES deviceid, out_nibits, out_found
    .if                                               \
        deviceid = NSK::DEVICES::SNES::CONTROLLERS || \
        deviceid = NSK::DEVICES::SNES::MOUSE_P4016 || \
        deviceid = NSK::DEVICES::SNES::MOUSE_P4017

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_iftaito
; @brief Pass through Taito “Vaus” (Arkanoid) paddles (NES/FC, prototype, with 
; Data Recorder bundle).
;
; @param    deviceid     Input deviceid symbol (NSK::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_iftaito deviceid, out_nibits, out_found
    .if                                                           \
        deviceid = NSK::DEVICES::TAITO::VAUS                   || \
        deviceid = NSK::DEVICES::TAITO::VAUS_PROTO             || \
        deviceid = NSK::DEVICES::TAITO::VAUS_DUAL_DATAREC

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_ifclonekbm
; @brief Pass through assorted third-party / clone / OEM devices
; (non-keyboard/mouse group)
;
; @param    deviceid     Input deviceid symbol (NSK::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifclonekbm deviceid, out_nibits, out_found
    .if                                                            \
        deviceid = NSK::DEVICES::ABL::PINBALL                   || \
        deviceid = NSK::DEVICES::ADVANCEDBRIGHT::CROAKYKARAOKE  || \
        deviceid = NSK::DEVICES::ASCII::TURBOFILE               || \
        deviceid = NSK::DEVICES::BRODERBUND::UFORCE             || \
        deviceid = NSK::DEVICES::CAPCOM::MAHJONG                || \
        deviceid = NSK::DEVICES::COCONUTS::PACHINKO             || \
        deviceid = NSK::DEVICES::ELECOM::MAHJONG                || \
        deviceid = NSK::DEVICES::EPOCH::BARCODEBATTLER2         || \
        deviceid = NSK::DEVICES::EXCALIBUR::SUDOKU              || \
        deviceid = NSK::DEVICES::GIGGGLE::PUMP                  || \
        deviceid = NSK::DEVICES::HUMMER::FISHINGROD             || \
        deviceid = NSK::DEVICES::IBM::KEYBOARD                  || \
        deviceid = NSK::DEVICES::LG::TVREMOTE                   || \
        deviceid = NSK::DEVICES::MAJESCO::CASINOCONTROLLER      || \
        deviceid = NSK::DEVICES::NICHIBUTSU::DOUBLE             || \
        deviceid = NSK::DEVICES::RACERMATE::BICYCLE             || \
        deviceid = NSK::DEVICES::SHARP::CASSETTE                || \
        deviceid = NSK::DEVICES::TIMETOP::LIGHTGUN              || \
        deviceid = NSK::DEVICES::TOOLWORKS::MIRACLEPIANO        || \
        deviceid = NSK::DEVICES::TOUCHGAMEPLAYER::MAHJONG       || \
        deviceid = NSK::DEVICES::VARIE::TOPRIDER                || \
        deviceid = NSK::DEVICES::VENOM::DANCEMAT                || \
        deviceid = NSK::DEVICES::YONEZAWA::PARTYTAP

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
; @param    deviceid     Input deviceid symbol (NSK::DEVICES::xxx).
; @param    out_nibits   Symbol assigned to the resulting 6-bit value.
; @param    out_found    Flag set to 1 if matched (unchanged otherwise).
.macro _convert_ifother deviceid, out_nibits, out_found
    .if                                                               \
        deviceid = NSK::DEVICES::BITCORP::KEYBOARD                 || \
        deviceid = NSK::DEVICES::BUBUGAO::KEYBOARD_MOUSE_P4017     || \
        deviceid = NSK::DEVICES::DONGDA::KEYBOARD                  || \
        deviceid = NSK::DEVICES::KEDA::KEYBOARD                    || \
        deviceid = NSK::DEVICES::KEWANG::KEYBOARD                  || \
        deviceid = NSK::DEVICES::UMC::KEYBOARD_MOUSE_P4017         || \
        deviceid = NSK::DEVICES::UMC::MOUSE                        || \
        deviceid = NSK::DEVICES::YUXING::MOUSE_P4016               || \
        deviceid = NSK::DEVICES::ZECHENG::KEYBOARD                 || \
        deviceid = NSK::DEVICES::SUBOR::KEYBOARD                   || \
        deviceid = NSK::DEVICES::SUBOR::KEYBOARD_MOUSE             || \
        deviceid = NSK::DEVICES::SUBOR::KEYBOARD_MOUSE_P4016       || \
        deviceid = NSK::DEVICES::SUBOR::KEYBOARD_MOUSE_P4017       || \
        deviceid = NSK::DEVICES::SUBOR::KEYBOARD_MOUSEPS2_P4017    || \
        deviceid = NSK::DEVICES::SUBOR::KEYBOARD_MOUSEPS2INV_P4017 || \
        deviceid = NSK::DEVICES::SUBOR::KEYBOARD_MOUSEYUX_P4016

        out_nibits = deviceid
        out_found .set 1
    .endif
.endmacro

; @macro _convert_deviceid
; @brief Resolve a deviceid symbol into the final 8-bit header value.
;
; Tries all vendor/family-specific converters. If none match, emits an error
;
; @param    deviceid     Input deviceid symbol (one of NSK::DEVICES::*).
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
; @param    deviceid  Input deviceid symbol (NSK::DEVICES::xxx).
.macro nsk_header_device deviceid
    ; locals:
    .local nibits_deviceid

    ; convert:
    _convert_deviceid deviceid, nibits_deviceid

    ; write:
    .byte nibits_deviceid
.endmacro

.endif
