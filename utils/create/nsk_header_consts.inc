; @file nsk_header_consts.inc
; @brief Constant definitions and enumerations for NES 2.0 headers.
;
; Provides named values for flags, and configuration
; options referenced in `nsk_header_config.inc`.
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_CONSTS__
::__NSK_HEADER_CONSTS__ = 1

; @enum nsk
; @brief Project-wide prefix
.scope nsk
    ; @enum nsk::nametable
    ; @brief Nametable settings
    .scope nametable
        ; @enum nsk::nametable::layout
        ; @brief Nametable arrangement / mirroring modes.
        ;
        ; Nametable mirroring controls how the PPU maps its 2 KiB of VRAM
        ; into the four logical nametables ($2000, $2400, $2800, $2C00).
        ; 
        ; - Vertical mirroring: NT0 = $2000, NT1 = $2400, both mirrored into
        ;   $2800/$2C00. Two unique vertical columns of 32×30 tiles.
        ; - Horizontal mirroring: NT0 = $2000, NT1 = $2800, both mirrored into
        ;   $2400/$2C00. Two unique horizontal rows of 32×30 tiles.
        ;
        ; Effectively, mirroring determines whether VRAM provides
        ; two screens stacked vertically or horizontally, with the
        ; other two screens being duplicates.
        ;
        ; Some boards are exceptions: certain cartridges wire only a single
        ; physical nametable, which is mirrored into all four logical slots.
        ; On the other end, advanced mappers can provide four fully
        ; independent nametables, giving the PPU complete flexibility
        ; without any mirroring.
        ; 
        ; See https://www.nesdev.org/wiki/Mirroring#Nametable_Mirroring for
        ; more info
        .scope layout
            ; @var vertical_arrangement
            ; @brief Vertical arrangement
            vertical_arrangement = 0
            ; @var horizontal_arrangement
            ; @brief Horizontal arrangement
            horizontal_arrangement = 1
            ; @var vertical_mirroring
            ; @brief Standard term: vertical mirroring
            vertical_mirroring = nsk::nametable::alignment::horizontal_arrangement
            ; @var horizontal_mirroring
            ; @brief Standard term: horizontal mirroring
            horizontal_mirroring = nsk::nametable::alignment::vertical_arrangement
        .endscope

        ; @enum nsk::nametable::mode
        ; @brief Alternative mirroring flag.
        .scope mode
            ; @var normal
            ; @brief Normal mirroring configuration.
            normal = 0
            ; @var alternative
            ; @brief Alternative mirroring configuration.
            alternative = 1
        .endscope
    .endscope


    ; @enum nsk::battery
    ; @brief Presence of battery-backed RAM on the cartridge.
    .scope battery
        ; @var absent
        ; @brief Cartridge has no battery-backed RAM.
        absent = 0
        ; @var present
        ; @brief Cartridge includes battery-backed RAM.
        present = 1
    .endscope

    ; @enum nsk::trainer
    ; @brief Presence of a 512-byte trainer at $7000–$71FF.
    .scope trainer
        ; @var absent
        ; @brief Cartridge does not include a trainer.
        absent = 0
        ; @var present
        ; @brief Cartridge includes a trainer section.
        present = 1
    .endscope

    ; @enum nsk::console
    ; @brief Console type identifiers for NES 2.0 header Flags 7.
    ;
    ; Encodes the target hardware family of the ROM image. 
    .scope console
        ; @var famicom
        ; @brief Nintendo Family Computer (Famicom).
        famicom = $00
        ; @var NES
        ; @brief Nintendo Entertainment System (NTSC/PAL). Alias of @ref console::famicom.
        NES = nsk::console::famicom
        ; @var dendy
        ; @brief Dendy (Russian NES clone). Alias of @ref console::famicom.
        dendy = nsk::console::famicom
        ; @var nintendo_vs_system
        ; @brief Nintendo VS. System arcade hardware.
        nintendo_vs_system = $01
        ; @var nintendo_playchoice_10
        ; @brief Nintendo PlayChoice-10 arcade system.
        nintendo_playchoice_10 = $02
        ; @var famiclone_decimal
        ; @brief Generic famiclone with decimal-based PPU.
        famiclone_decimal = $03
        ; @var famicom_epsm
        ; @brief Famicom clone with extended sound (EPSM).
        famicom_epsm = $04
        ; @var UMC_UM6578
        ; @brief UMC UM6578 SoC.
        UMC_UM6578 = $0b
        ; @var famicom_network_system
        ; @brief Famicom Network System (Japan-only add-on).
        famicom_network_system = $0c

        ; @enum nsk::console::VR
        ; @brief VR Technology consoles
        .scope VR
            ; @var VT01
            ; @brief VT01 SoC (basic NOAC clone).
            VT01 = $05
            ; @var VT02
            ; @brief VT02 SoC.
            VT02 = $06
            ; @var VT03
            ; @brief VT03 SoC.
            VT03 = $07
            ; @var VT09
            ; @brief VT09 SoC.
            VT09 = $08
            ; @var VT32
            ; @brief VT32 SoC.
            VT32 = $09
            ; @var VT369
            ; @brief VT369 SoC.
            VT369 = $0a
        .endscope

        ; @enum nsk::console::VS
        ; @brief VS console additional constants
        .scope VS
            ; @enum nsk::console::VS::ppu
            ; @brief VS console PPU types
            .scope ppu
                ; @var RP2C03
                ; @brief RP2C03 PPU chip
                RP2C03 = $00
                ; @var RC2C03
                ; @brief RC2C03 PPU chip
                RC2C03 = nsk::console::VS::ppu::RP2C03

                ; @enum nsk::console::VS::ppu::RP2C04
                ; @brief RP2C04 PPU chip
                .scope RP2C04
                    ; @var rev1
                    ; @brief Revision 1
                    rev1 = $02
                    ; @var rev2
                    ; @brief Revision 2
                    rev2 = $03
                    ; @var rev3
                    ; @brief Revision 3
                    rev3 = $04
                    ; @var rev4
                    ; @brief Revision 4
                    rev4 = $05
                .endscope

                ; @enum nsk::console::RC2C05
                ; @brief RC2C05 PPU chip
                .scope RC2C05
                    ; @var rev1
                    ; @brief Revision 1
                    rev1 = $08
                    ; @var rev2
                    ; @brief Revision 2
                    rev2 = $09
                    ; @var rev3
                    ; @brief Revision 3
                    rev3 = $0a
                    ; @var rev4
                    ; @brief Revision 4
                    rev4 = $0b
                .endscope
            .endscope

            ; @enum nsk::console::VS::hardware
            ; @brief VS console hardware types
            .scope hardware
                ; @enum nsk::console::VS::hardware::unisystem
                ; @brief Unisystem hardware types
                .scope unisystem
                    ; @var normal
                    ; @brief Standard Vs. Unisystem hardware (0).
                    normal  = $00
                    ; @var RBI_baseball
                    ; @brief Vs. Unisystem with RBI Baseball protection
                    RBI_baseball = $01
                    ; @var TKO_boxing
                    ; @brief Vs. Unisystem with TKO Boxing protection
                    TKO_boxing = $02
                    ; @var Super_Xevious
                    ; @brief Vs. Unisystem with Super Xevious protection
                    super_xevious = $03
                    ; @var ice_climber_japan
                    ; @brief Vs. Unisystem with Ice Climber Japan protection
                    ice_climber_japan = $04
                .endscope

                ; @enum nsk::console::VS::hardware::dualsystem
                ; @enum Dualsystem hardware types
                .scope dualsystem
                    ; @var normal
                    ; @brief Standard Vs. DualSystem hardware 
                    normal = $05
                    ; @var raid_on_bungeling_bay
                    ; @brief Vs. DualSystem with Raid on Bungeling Bay protection
                    raid_on_bungeling_bay = $06
                .endscope
            .endscope
        .endscope
    .endscope

    ; @enum nsk::region
    ; @brief Region and PPU identifiers for NES 2.0 header configuration.
    ;
    ; Encodes the intended video standard / PPU variant used by the ROM.
    ; These values inform the emulator or hardware which timing and
    ; region-specific behavior to apply.
    ;
    ; Multiple aliases are provided for human readability, all resolving
    ; to the same underlying constant.
    .scope region
        ; @var RP2C02
        ; @brief NTSC PPU used in Japan and North America
        ; 
        ; Licensed NTSC 60 Hz 
        RP2C02 = $00
        ; @var NTSC
        ; @brief Alias of @ref region::RP2C02
        NTSC = nsk::region::RP2C02
        ; @var North_America
        ; @brief Alias of @ref region::RP2C02
        North_America = nsk::region::RP2C02
        ; @var Japan
        ; @brief Alias of @ref region::RP2C02
        Japan = nsk::region::RP2C02
        ; @var South_Korea
        ; @brief Alias of @ref region::RP2C02
        South_Korea = nsk::region::RP2C02
        ; @var Taiwan
        ; @brief Alias of @ref region::RP2C02
        Taiwan = nsk::region::RP2C02

        ; @var RP2C07
        ; @brief PAL PPU used in Western Europe and Australia
        ;
        ; Licensed PAL 50 Hz 
        RP2C07 = $01
        ; @var PAL
        ; @brief Alias of @ref region::RP2C07
        PAL = nsk::region::RP2C07
        ; @var Western_Europe
        ; @brief Alias of @ref region::RP2C07
        Western_Europe = nsk::region::RP2C07
        ; @var Australia
        ; @brief Alias of @ref region::RP2C07
        Australia = nsk::region::RP2C07

        ; @var Multiple
        ; @brief Multiregion / dual-PPU cartridge support
        Multiple = $02

        ; @var UA6538
        ; @brief Dendy-compatible PAL clone PPU, common in E.Europe, Russia, etc
        UA6538 = $03
        ; @var Dendy
        ; @brief Alias of @ref region::UA6538
        ;
        ; Unlicensed hybrid with 50 Hz PAL CPU and 60 Hz NTSC PPU
        Dendy = nsk::region::UA6538
        ; @var Eastern_Europe
        ; @brief Alias of @ref region::UA6538
        Eastern_Europe = nsk::region::UA6538
        ; @var Russia
        ; @brief Alias of @ref region::UA6538
        Russia = nsk::region::UA6538
        ; @var Mainland_China
        ; @brief Alias of @ref region::UA6538
        Mainland_China = nsk::region::UA6538
        ; @var India
        ; @brief Alias of @ref region::UA6538
        India = nsk::region::UA6538
        ; @var Africa
        ; @brief Alias of @ref region::UA6538
        Africa = nsk::region::UA6538
    .endscope

    ; @enum nsk::device
    ; @brief Expansion devices list
    .scope device
        ; @var nsk::device::unspecified
        ; @brief Unspecified/none. No special expansion device is declared
        unspecified = $00

        ; @var nsk::device::multicart
        ; @brief Multicart selector logic present; games expect menu/selector 
        ; hardware
        multicart = $2a

        ; @enum nsk::device::nintendo
        ; @brief Official Famicom/NES controllers
        .scope nintendo
            ; @var nsk::device::famicom::controller
            ; @brief Standard NES/Famicom digital controllers (2 pads).
            controller = $01
            ; @var nsk::device::famicom::controller_inv
            ; @brief Standard Controller with swapped Left–Right / Up–Down / B–A
            ; Rare/debug-oriented mapping used by some service/test contexts.
            controller_inv = $31
            ; @var nsk::device::famicom::controller_test
            ; @brief Port test controller (diagnostic device).
            ; Used in factory/service ROMs to verify ports/buttons.
            controller_test = $37
            ; @var nsk::device::famicom::fourplayers_simple
            ; @brief Famicom Four Players Adapter (simple protocol).
            ; Four-player titles using the simple 15-pin EXP protocol.
            fourplayers_simple = $03
            ; @var nsk::device::famicom::keyboardsilent_datarecorder
            ; @brief Famicom Data Recorder with “silent” keyboard
            keyboardsilent_datarecorder = $20
            ; @var nsk::device::famicom::keyboardBASIC_datarecorder
            ; @brief Family BASIC Keyboard + Famicom Data Recorder bundle.
            keyboardBASIC_datarecorder = $23
            ; @var nsk::device::famicom::network_controller
            ; @brief Famicom Network Controller (modem/remote control family).
            ; Rare; used with network/TV control experiments in JP.
            network_controller = $3b
            ; @var nsk::device::famicom::ROB_gyro
            ; @brief R.O.B. Gyro Set (ROB support device used by Gyromite).
            ROB_gyro = $1f
            ; @var nsk::device::famicom::ROB_stackup
            ; @brief R.O.B. Stack-Up (ROB support device used by Stack-Up).
            ROB_stackup = $2e
            ; @var nsk::device::famicom::zapper_4016
            ; @brief Zapper on port $4016 (NES-style pinout through adapter).
            zapper_4016 = $49
            ; @var nsk::device::famicom::zapper_4017
            ; @brief Zapper on port $4017 (EXP-port).
            zapper_4017 = $08
            ; @var nsk::device::famicom::zapper_dual
            ; @brief Two Zappers connected (dual lightguns).
            ; Dual-zapper test/homebrew; limited licensed usage.
            zapper_dual = $09
            ; @var nsk::device::famicom::3dsystem
            ; @brief Famicom 3D System (LCD shutter glasses).
            ; JJ/Tobidase Daisakusen, Falsion, 3D Hot Rally etc.
            ; @note OUT0 toggles eye; 30 Hz per-eye flicker.
            3dsystem = $1d
            ; @var nsk::device::nes::fourscore
            ; @brief NES Four Score / Satellite (4-player hub) + two standard 
            ; pads.
            fourscore = $02
            ; @var nsk::device::nes::powerpad_sidea
            ; @brief Power Pad (Side A) — NES version of Family Trainer.
            powerpad_sidea = $0b
            ; @var nsk::device::nes::powerpad_sideb
            ; @brief Power Pad (Side B) — NES version of Family Trainer.
            powerpad_sideb = $0c; 
            ; @var nsk::device::nes::vaus
            ; @brief Arkanoid “Vaus” paddle (NES version).
            vaus = $0f
        .endscope

        ; @enum nsk::device::VS
        ; @brief VS System (Uni/Dualsystem) controllers
        .scope VS
            ; @enum nsk::device::VS::system_4016
            ; @brief Vs. System, 1P via $4016 (arcade VS Unisystem).
            ; @notes Special mapping of the VS System controller
            system_4016 = $04
            ; @var nsk::device::VS::system_4017
            ; @brief Vs. System, 1P via $4017
            system_4017 = $05
            ; @var nsk::device::VS::zapper
            ; @brief Vs. System zapper (incompatible with the famicom zapper)
            zapper = $07
        .endscope

        ; @enum nsk::device::bandai
        ; @brief Bandai manufactured controllers
        .scope bandai
            ; @var nsk::device::bandai::cookingplayset
            ; @brief “Magical Cooking” Bandai playset (plug-and-play based on 
            ; UM6578).
            ; Toy kitchen playset with an embedded Famiclone game
            cookingplayset = $47
            ; @var nsk::device::bandai::familytrainer_sidea
            ; @brief Family Trainer (Exercise Mat) — Side A.
            ; Family Trainer series (JP). NES counterpart: Power Pad.
            familytrainer_sidea = $0d
            ; @var nsk::device::bandai::familytrainer_sideb
            ; @brief Family Trainer (Exercise Mat) — Side B
            ; Family Trainer series (JP). NES counterpart: Power Pad.
            familytrainer_sideb = $0e
            ; @var nsk::device::bandai::gamepad
            ; @brief Bandai Multi Game Player — gamepad button mapping 
            ; (UM6578 PnP).
            gamepad = $38
            ; @var nsk::device::bandai::hypershot
            ; @brief Bandai Hyper Shot lightgun (for the "Space Shadow").
            hypershot = $0a
            ; @var nsk::device::bandai::tablet
            ; @brief Oeka Kids Tablet (Drawing tablet Oeka Kids series)
            tablet = $17
        .endscope

        ; @enum konami
        ; @brief Konami manufactured controllers
        .scope konami
            ; @var nsk::device::konami::doremikko
            ; @brief DoReMiKko Keyboard
            ; Music keyboard by Konami for the game Doremikko
            doremikko = $1e
            ; @var nsk::device::konami::hypershot
            ; @brief Konami Hyper Shot
            ; Dual trigger controller for Hyper Olympic/Sports.
            hypershot = $12
            ; @var nsk::device::konami::punchingbag
            ; @brief “Exciting Boxing” Punching Bag
            ; Inflatable boxing controller by Konami
            punchingbag = $14
        .endscope

        ; @enum nsk::device::IGS
        ; @brief Information Global Service manufactured controllers
        .scope IGS
            ; @var nsk::device::IGS::battlebox
            ; @brief IGS Battle Box — EXP device with 512-byte EEPROM storage.
            battlebox = $22
            ; @var nsk::device::IGS::pokkunmogura
            ; @brief Pokkun Moguraa (IGS) — “whack-a-mole” hammer + mat 
            ; controller.
            pokkunmogura = $1a
        .endscope

        ; @enum SNES
        ; @brief SNES controllers connected to NES
        .scope SNES
            ; @var nsk::device::snes::controllers
            ; @brief Two SNES controllers replacing the two standard NES 
            ; controllers (via adapter).
            controllers = $2b
            ; @var nsk::device::snes::mouse_4016
            ; @brief SNES Mouse via $4016 (through an adapter).
            mouse_4016 = $29
            ; @var nsk::device::snes::mouse_4017
            ; @brief SNES Mouse via $4017 (through an adapter).
            mouse_4017 = $48
        .endscope

        ; @enum taito
        ; @brief Taito manufactured controllers
        .scope taito
            ; @var nsk::device::taito::vaus
            ; @brief Arkanoid “Vaus” paddle (Famicom version).
            ; Arkanoid II (FC) and others; distinct from NES wiring.
            vaus = $10
            ; @var nsk::device::taito::vaus_proto
            ; @brief Arkanoid Vaus (Prototype variant).
            ; Prototype/early hardware ID for Vaus-like paddle.
            vaus_proto = $4a
            ; @var nsk::device::taito::vausdual_datarecorder
            ; @brief Two Vaus controllers plus Famicom Data Recorder
            ; Arkanoid II exposes its own EXP port; recorder must be on main EXP
            vausdual_datarecorder = $11
        .endscope

        ; @var nsk::device::abl_pinball
        ; @brief ABL Pinball — extra pinball buttons controller.
        abl_pinball = $33
        ; @var nsk::device::advancebright_croakykaraoke
        ; @brief “Croaky Karaoke” — karaoke mic/remote by Advance Bright.
        advancebright_croakykaraoke = $3d
        ; @var nsk::device::ascii_turbofile
        ; @brief ASCII Turbo File
        ; External non-volatile save storage.
        ascii_turbofile = $21
        ; @var nsk::device::broderbund_uforce
        ; @brief Brøderbund U-Force
        ; Infrared “hands-free” controller panel
        broderbund_uforce = $2d
        ; @var nsk::device::capcom_mahjong
        ; @brief Jissen Mahjong Controller
        ; 21-button mahjong panel used by "Ide Yousuke Meijin no Jissen 
        ; Mahjong" (1 and 2)
        capcom_mahjong = $15
        ; @var nsk::device::coconuts_pachinko
        ; @brief Pachinko controller (Coconuts Japan).
        ; Used by several "Pachinko" titles
        coconuts_pachinko = $13
        ; @var nsk::device::elecom_mahjong
        ; @brief 麻雀激闘伝説 (Mahjong Gekitou Densetsu) Controller
        ; The only reference to its manufacturer and the inners is the
        ; http://yahooland.seesaa.net/article/a1699780.html
        elecom_mahjong = $4c
        ; @var nsk::device::epoch_barcodebattler2
        ; @brief Barcode Battler II interface (Epoch), as used by 
        ; Sunsoft’s Barcode World. Used by "Barcode World" (1992, Famicom).
        epoch_barcodebattler2 = $18
        ; @var nsk::device::excalibur_sudoku
        ; @brief Excalibur Sudoku Pad (clone numeric tablet for Sudoku software)
        excalibur_sudoku = $32
        ; @var nsk::device::gigggle_pump
        ; @brief Gigggle TV Pump — pump fitness controller (plug-and-play).
        ; @note It's "Gigggle", not "Giggle"
        gigggle_pump = $45
        ; @var nsk::device::hummer_fishingrod
        ; @brief King Fishing Controller — fishing-rod style controller (PnP).
        ; @notes Manufacturer branding varies
        hummer_fishingrod = $3c
        ; @var nsk::device::IBM_keyboard
        ; @brief IBM PC/XT keyboard
        ; PS/2-like serial interface via expansion
        IBM_keyboard = $4e
        ; @var nsk::device::LG_tvremote
        ; @brief LG TV Remote — IR remote control for TVs with an embedded 
        ; Famiclone.
        LG_tvremote = $3a
        ; @var nsk::device::majesco_casinocontroller
        ; @brief Golden Nugget Casino extra buttons (Majesco release).
        majesco_casinocontroller = $34
        ; @var nsk::device::nichibutsu_4players_bothhands
        ; @brief “Double-fisted” play
        ; One player uses two controllers at once (Crazy Climber/Smash TV style)
        ; @note Often relies on Four Score; some homebrew/ports support it 
        ; explicitly.
        nichibutsu_double_fourscore = $1c
        ; @var nsk::device::racermate_bicycle
        ; @brief RacerMate Bicycle — exercise bike interface.
        ; Used by "RacerMate Challenge" hardware/software.
        racermate_bicycle = $2c
        ; @var nsk::device::sharp_cassette
        ; @brief Sharp C1 Cassette Interface (tape I/O interface for JP setups).
        sharp_cassette = $30
        ; @var nsk::device::timetop_lightgun
        ; @brief City Patrolman Lightgun (TimeTop; clone lightgun).
        timetop_lightgun = $2f
        ; @var nsk::device::toolworks_miraclepiano
        ; @brief Miracle Piano (Antex for The Software Toolworks)
        ; 49-key MIDI keyboard/teaching system. Used by "The Miracle Piano 
        ; Teaching System" (NES cartridge)
        toolworks_miraclepiano = $19
        ; @var nsk::device::touchgameplayer_mahjong
        ; @brief TV 麻雀 Game (TV Mahjong Game) Controller
        ; Mahjong plug & play produced for the Japanese market; appears to 
        ; play a bootleg hack of Capcom's Ide Yousuke Meijin no Jissen Mahjong
        touchgameplayer_mahjong = $4b
        ; @var nsk::device::varie_toprider
        ; @brief Top Rider (Varie)
        ; Inflatable “bike” controller with steering and sensors. Used-by 
        ; "Top Rider" (Famicom; bundled accessory).
        varie_toprider = $1b
        ; @var nsk::device::venom_dancemat
        ; @brief Venom TV Dance Mat (plug-and-play 4-in-1 dance pad).
        venom_dancemat = $39
        ; @var nsk::device::yonezawa_partytap
        ; @brief Yonezawa Party Tap (six “buzzer” panels for quiz-style games).
        ; Used by "Gimme A Break! Shijō Saikyō no Quiz-Ō Ketteisen" and others.
        yonezawa_partytap = $16

        ; @enum nsk::device::clonekbm
        ; @brief Other alternative famiclone keyboard and mouse controllers
        .scope clonekbm
            ; @var nsk::device::clonekbm::bitcorp_keyboard
            ; @brief Puze / Bit Corp. (普澤 / Bit-79) keyboard
            bitcorp_keyboard = $25
            ; @var nsk::device::clonekbm::bubugao_keyboard_mouse_4017
            ; @brief Bùbùgāo (步步高, BBK) keyboard + R90-rotated PS/2 mouse via
            ; $4017.
            bubugao_keyboard_mouse_4017 = $46
            ; @var nsk::device::clonekbm::dongda_keyboard
            ; @brief Dongda (东达) PEC Keyboard
            dongda_keyboard = $24
            ; @var nsk::device::clonekbm::keda_keyboard
            ; @brief Kēdá (科达) keyboard
            keda_keyboard = $35
            ; @var nsk::device::clonekbm::kewang_keyboard
            ; @brief Kēwáng (科王, Kingwon) keyboard.
            kewang_keyboard = $3e
            ; @var nsk::device::clonekbm::UMC_keyboard_mouse_4017
            ; @brief PS/2 keyboard in UM6578 PS/2 port + PS/2 mouse via $4017.
            UMC_keyboard_mouse_4017 = $41
            ; @var nsk::device::clonekbm::UMC_mouse
            ; @brief PS/2 mouse in UM6578 PS/2 port.
            UMC_mouse = $42
            ; @var nsk::device::clonekbm::yuxing_mouse_4016
            ; @brief Yùxìng (裕兴) mouse via $4016.
            yuxing_mouse_4016 = $43
            ; @var nsk::device::clonekbm::zecheng_keyboard
            ; @brief Zéchéng (泽诚) keyboard.
            zecheng_keyboard = $3f

            ; @enum nsk::device::clonekbm::SUBOR
            ; @brief SUBOR manufactured controllers
            .scope SUBOR
                ; @var nsk::device::clonekbm::SUBOR::keyboard
                ; @brief Subor (小霸王) keyboard (typical Famiclone keyboard).
                keyboard = $26
                ; @var nsk::device::clonekbm::SUBOR::keyboard_mouse
                ; @brief Subor keyboard + Macro Winners mouse (bundle).
                keyboard_mouse = $27
                ; @var nsk::device::clonekbm::SUBOR::keyboard_mouse_4016
                ; @brief Subor keyboard + Subor mouse via $4016.
                keyboard_mouse_4016 = $28
                ; @var nsk::device::clonekbm::SUBOR::keyboard_mouse_4017
                ; @brief Subor keyboard + Subor mouse via $4017.
                keyboard_mouse_4017 = $36
                ; @var nsk::device::clonekbm::SUBOR::keyboard_mousePS2_4017
                ; @brief Subor keyboard + PS/2 mouse (L90-rotated) via $4017.
                ; “L90 rotated” indicates axes rotated by 90° in the mouse 
                ; report.
                keyboard_mousePS2_4017 = $40
                ; @var nsk::device::clonekbm::SUBOR::keyboard_mousePS2inv_4017
                ; @brief Subor keyboard + X-inverted PS/2 mouse via $4017.
                ; @note X-axis is inverted in the mouse report.
                keyboard_mousePS2inv_4017 = $4d
                ; @var nsk::device::clonekbm::SUBOR::keyboard_mouseyux_4016
                ; @brief Subor keyboard + Yuxing (裕兴) mouse via $4016.
                keyboard_mouseyux_4016 = $44
            .endscope
        .endscope
    .endscope
.endscope

.endif
