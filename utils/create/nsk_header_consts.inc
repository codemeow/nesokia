; @file nsk_header_consts.inc
; @brief Constant definitions and enumerations for NES 2.0 headers.
;
; Provides named values for flags, and configuration
; options referenced in `nsk_header_config.inc`.
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_HEADER_CONSTS__
::__NSK_HEADER_CONSTS__ = 1

; @brief Project-wide prefix
.scope nsk
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
            ; @brief Vertical arrangement
            vertical_arrangement = 0
            ; @brief Horizontal arrangement
            horizontal_arrangement = 1
            ; @brief Standard term: vertical mirroring
            vertical_mirroring = nsk::nametable::layout::horizontal_arrangement
            ; @brief Standard term: horizontal mirroring
            horizontal_mirroring = nsk::nametable::layout::vertical_arrangement
        .endscope

        ; @brief Alternative mirroring flag.
        .scope mode
            ; @brief Normal mirroring configuration.
            normal = 0
            ; @brief Alternative mirroring configuration.
            alternative = 1
        .endscope
    .endscope


    ; @brief Presence of battery-backed RAM on the cartridge.
    .scope battery
        ; @brief Cartridge has no battery-backed RAM.
        absent = 0
        ; @brief Cartridge includes battery-backed RAM.
        present = 1
    .endscope

    ; @brief Presence of a 512-byte trainer at $7000–$71FF.
    .scope trainer
        ; @brief Cartridge does not include a trainer.
        absent = 0
        ; @brief Cartridge includes a trainer section.
        present = 1
    .endscope

    ; @brief Console type identifiers for NES 2.0 header Flags 7.
    ;
    ; Encodes the target hardware family of the ROM image. 
    .scope console
        ; @brief Nintendo Family Computer (Famicom).
        famicom = $00
        ; @brief Nintendo Entertainment System (NTSC/PAL). Alias of @ref console::famicom.
        NES = nsk::console::famicom
        ; @brief Dendy (Russian NES clone). Alias of @ref console::famicom.
        dendy = nsk::console::famicom
        ; @brief Nintendo VS. System arcade hardware.
        nintendo_vs_system = $01
        ; @brief Nintendo PlayChoice-10 arcade system.
        nintendo_playchoice_10 = $02
        ; @brief Generic famiclone with decimal-based PPU.
        famiclone_decimal = $03
        ; @brief Famicom clone with extended sound (EPSM).
        famicom_epsm = $04
        ; @brief UMC UM6578 SoC.
        UMC_UM6578 = $0b
        ; @brief Famicom Network System (Japan-only add-on).
        famicom_network_system = $0c

        ; @brief VR Technology consoles
        .scope VR
            ; @brief VT01 SoC (basic NOAC clone).
            VT01 = $05
            ; @brief VT02 SoC.
            VT02 = $06
            ; @brief VT03 SoC.
            VT03 = $07
            ; @brief VT09 SoC.
            VT09 = $08
            ; @brief VT32 SoC.
            VT32 = $09
            ; @brief VT369 SoC.
            VT369 = $0a
        .endscope

        ; @brief VS console additional constants
        .scope VS
            ; @brief VS console PPU types
            .scope ppu
                ; @brief Not applicable
                not_applicable = $00

                ; @brief RP2C03 PPU chip
                RP2C03 = $00
                ; @brief RC2C03 PPU chip
                RC2C03 = nsk::console::VS::ppu::RP2C03

                ; @brief RP2C04 PPU chip
                .scope RP2C04
                    ; @brief Revision 1
                    rev1 = $02
                    ; @brief Revision 2
                    rev2 = $03
                    ; @brief Revision 3
                    rev3 = $04
                    ; @brief Revision 4
                    rev4 = $05
                .endscope

                ; @brief RC2C05 PPU chip
                .scope RC2C05
                    ; @brief Revision 1
                    rev1 = $08
                    ; @brief Revision 2
                    rev2 = $09
                    ; @brief Revision 3
                    rev3 = $0a
                    ; @brief Revision 4
                    rev4 = $0b
                .endscope
            .endscope

            ; @brief VS console hardware types
            .scope hardware
                ; @brief Not applicable
                not_applicable = $00

                ; @brief Unisystem hardware types
                .scope unisystem
                    ; @brief Standard Vs. Unisystem hardware (0).
                    normal  = $00
                    ; @brief Vs. Unisystem with RBI Baseball protection
                    RBI_baseball = $01
                    ; @brief Vs. Unisystem with TKO Boxing protection
                    TKO_boxing = $02
                    ; @brief Vs. Unisystem with Super Xevious protection
                    super_xevious = $03
                    ; @brief Vs. Unisystem with Ice Climber Japan protection
                    ice_climber_japan = $04
                .endscope

                ; @enum Dualsystem hardware types
                .scope dualsystem
                    ; @brief Standard Vs. DualSystem hardware 
                    normal = $05
                    ; @brief Vs. DualSystem with Raid on Bungeling Bay protection
                    raid_on_bungeling_bay = $06
                .endscope
            .endscope
        .endscope
    .endscope

    ; @brief Region and PPU identifiers for NES 2.0 header configuration.
    ;
    ; Encodes the intended video standard / PPU variant used by the ROM.
    ; These values inform the emulator or hardware which timing and
    ; region-specific behavior to apply.
    ;
    ; Multiple aliases are provided for human readability, all resolving
    ; to the same underlying constant.
    .scope region
        ; @brief NTSC PPU used in Japan and North America
        ; Licensed NTSC 60 Hz 
        RP2C02 = $00
        ; @brief Alias of @ref region::RP2C02
        NTSC = nsk::region::RP2C02
        ; @brief Alias of @ref region::RP2C02
        North_America = nsk::region::RP2C02
        ; @brief Alias of @ref region::RP2C02
        Japan = nsk::region::RP2C02
        ; @brief Alias of @ref region::RP2C02
        South_Korea = nsk::region::RP2C02
        ; @brief Alias of @ref region::RP2C02
        Taiwan = nsk::region::RP2C02

        ; @brief PAL PPU used in Western Europe and Australia
        ; Licensed PAL 50 Hz 
        RP2C07 = $01
        ; @brief Alias of @ref region::RP2C07
        PAL = nsk::region::RP2C07
        ; @brief Alias of @ref region::RP2C07
        Western_Europe = nsk::region::RP2C07
        ; @brief Alias of @ref region::RP2C07
        Australia = nsk::region::RP2C07

        ; @brief Multiregion / dual-PPU cartridge support
        Multiple = $02

        ; @brief Dendy-compatible PAL clone PPU, common in E.Europe, Russia, etc
        UA6538 = $03
        ; @brief Alias of @ref region::UA6538
        ; Unlicensed hybrid with 50 Hz PAL CPU and 60 Hz NTSC PPU
        Dendy = nsk::region::UA6538
        ; @brief Alias of @ref region::UA6538
        Eastern_Europe = nsk::region::UA6538
        ; @brief Alias of @ref region::UA6538
        Russia = nsk::region::UA6538
        ; @brief Alias of @ref region::UA6538
        Mainland_China = nsk::region::UA6538
        ; @brief Alias of @ref region::UA6538
        India = nsk::region::UA6538
        ; @brief Alias of @ref region::UA6538
        Africa = nsk::region::UA6538
    .endscope

    ; @brief Expansion devices list
    .scope device
        ; @brief Unspecified/none. No special expansion device is declared
        unspecified = $00

        ; @brief Multicart selector logic present; games expect menu/selector 
        ; hardware
        multicart = $2a

        ; @brief Official Famicom/NES controllers
        .scope nintendo
            ; @brief Standard NES/Famicom digital controllers (2 pads).
            controller = $01
            ; @brief Standard Controller with swapped Left–Right / Up–Down / B–A
            ; Rare/debug-oriented mapping used by some service/test contexts.
            controller_inv = $31
            ; @brief Port test controller (diagnostic device).
            ; Used in factory/service ROMs to verify ports/buttons.
            controller_test = $37
            ; @brief Famicom Four Players Adapter (simple protocol).
            ; Four-player titles using the simple 15-pin EXP protocol.
            fourplayers_simple = $03
            ; @brief Famicom Data Recorder with “silent” keyboard
            keyboardsilent_datarecorder = $20
            ; @brief Family BASIC Keyboard + Famicom Data Recorder bundle.
            keyboardBASIC_datarecorder = $23
            ; @brief Famicom Network Controller (modem/remote control family).
            ; Rare; used with network/TV control experiments in JP.
            network_controller = $3b
            ; @brief R.O.B. Gyro Set (ROB support device used by Gyromite).
            ROB_gyro = $1f
            ; @brief R.O.B. Stack-Up (ROB support device used by Stack-Up).
            ROB_stackup = $2e
            ; @brief Zapper on port $4016 (NES-style pinout through adapter).
            zapper_4016 = $49
            ; @brief Zapper on port $4017 (EXP-port).
            zapper_4017 = $08
            ; @brief Two Zappers connected (dual lightguns).
            ; Dual-zapper test/homebrew; limited licensed usage.
            zapper_dual = $09
            ; @brief Famicom 3D System (LCD shutter glasses).
            ; JJ/Tobidase Daisakusen, Falsion, 3D Hot Rally etc.
            ; @note OUT0 toggles eye; 30 Hz per-eye flicker.
            system3d = $1d
            ; @brief NES Four Score / Satellite (4-player hub) + two standard 
            ; pads.
            fourscore = $02
            ; @brief Power Pad (Side A) — NES version of Family Trainer.
            powerpad_sidea = $0b
            ; @brief Power Pad (Side B) — NES version of Family Trainer.
            powerpad_sideb = $0c; 
            ; @brief Arkanoid “Vaus” paddle (NES version).
            vaus = $0f
        .endscope

        ; @brief VS System (Uni/Dualsystem) controllers
        .scope VS
            ; @brief Vs. System, 1P via $4016 (arcade VS Unisystem).
            ; @notes Special mapping of the VS System controller
            system_4016 = $04
            ; @brief Vs. System, 1P via $4017
            system_4017 = $05
            ; @brief Vs. System zapper (incompatible with the famicom zapper)
            zapper = $07
        .endscope

        ; @brief Bandai manufactured controllers
        .scope bandai
            ; @brief “Magical Cooking” Bandai playset (plug-and-play based on 
            ; UM6578).
            ; Toy kitchen playset with an embedded Famiclone game
            cookingplayset = $47
            ; @brief Family Trainer (Exercise Mat) — Side A.
            ; Family Trainer series (JP). NES counterpart: Power Pad.
            familytrainer_sidea = $0d
            ; @brief Family Trainer (Exercise Mat) — Side B
            ; Family Trainer series (JP). NES counterpart: Power Pad.
            familytrainer_sideb = $0e
            ; @brief Bandai Multi Game Player — gamepad button mapping 
            ; (UM6578 PnP).
            gamepad = $38
            ; @brief Bandai Hyper Shot lightgun (for the "Space Shadow").
            hypershot = $0a
            ; @brief Oeka Kids Tablet (Drawing tablet Oeka Kids series)
            tablet = $17
        .endscope

        ; @brief Konami manufactured controllers
        .scope konami
            ; @brief DoReMiKko Keyboard
            ; Music keyboard by Konami for the game Doremikko
            doremikko = $1e
            ; @brief Konami Hyper Shot
            ; Dual trigger controller for Hyper Olympic/Sports.
            hypershot = $12
            ; @brief “Exciting Boxing” Punching Bag
            ; Inflatable boxing controller by Konami
            punchingbag = $14
        .endscope

        ; @brief Information Global Service manufactured controllers
        .scope IGS
            ; @brief IGS Battle Box — EXP device with 512-byte EEPROM storage.
            battlebox = $22
            ; @brief Pokkun Moguraa (IGS) — “whack-a-mole” hammer + mat 
            ; controller.
            pokkunmogura = $1a
        .endscope

        ; @brief SNES controllers connected to NES
        .scope SNES
            ; @brief Two SNES controllers replacing the two standard NES 
            ; controllers (via adapter).
            controllers = $2b
            ; @brief SNES Mouse via $4016 (through an adapter).
            mouse_4016 = $29
            ; @brief SNES Mouse via $4017 (through an adapter).
            mouse_4017 = $48
        .endscope

        ; @brief Taito manufactured controllers
        .scope taito
            ; @brief Arkanoid “Vaus” paddle (Famicom version).
            ; Arkanoid II (FC) and others; distinct from NES wiring.
            vaus = $10
            ; @brief Arkanoid Vaus (Prototype variant).
            ; Prototype/early hardware ID for Vaus-like paddle.
            vaus_proto = $4a
            ; @brief Two Vaus controllers plus Famicom Data Recorder
            ; Arkanoid II exposes its own EXP port; recorder must be on main EXP
            vausdual_datarecorder = $11
        .endscope

        ; @brief ABL Pinball — extra pinball buttons controller.
        abl_pinball = $33
        ; @brief “Croaky Karaoke” — karaoke mic/remote by Advance Bright.
        advancebright_croakykaraoke = $3d
        ; @brief ASCII Turbo File
        ; External non-volatile save storage.
        ascii_turbofile = $21
        ; @brief Brøderbund U-Force
        ; Infrared “hands-free” controller panel
        broderbund_uforce = $2d
        ; @brief Jissen Mahjong Controller
        ; 21-button mahjong panel used by "Ide Yousuke Meijin no Jissen 
        ; Mahjong" (1 and 2)
        capcom_mahjong = $15
        ; @brief Pachinko controller (Coconuts Japan).
        ; Used by several "Pachinko" titles
        coconuts_pachinko = $13
        ; @brief 麻雀激闘伝説 (Mahjong Gekitou Densetsu) Controller
        ; The only reference to its manufacturer and the inners is the
        ; http://yahooland.seesaa.net/article/a1699780.html
        elecom_mahjong = $4c
        ; @brief Barcode Battler II interface (Epoch), as used by 
        ; Sunsoft’s Barcode World. Used by "Barcode World" (1992, Famicom).
        epoch_barcodebattler2 = $18
        ; @brief Excalibur Sudoku Pad (clone numeric tablet for Sudoku software)
        excalibur_sudoku = $32
        ; @brief Gigggle TV Pump — pump fitness controller (plug-and-play).
        ; @note It's "Gigggle", not "Giggle"
        gigggle_pump = $45
        ; @brief King Fishing Controller — fishing-rod style controller (PnP).
        ; @notes Manufacturer branding varies
        hummer_fishingrod = $3c
        ; @brief IBM PC/XT keyboard
        ; PS/2-like serial interface via expansion
        IBM_keyboard = $4e
        ; @brief LG TV Remote — IR remote control for TVs with an embedded 
        ; Famiclone.
        LG_tvremote = $3a
        ; @brief Golden Nugget Casino extra buttons (Majesco release).
        majesco_casinocontroller = $34
        ; @brief “Double-fisted” play
        ; One player uses two controllers at once (Crazy Climber/Smash TV style)
        ; @note Often relies on Four Score; some homebrew/ports support it 
        ; explicitly.
        nichibutsu_double_fourscore = $1c
        ; @brief RacerMate Bicycle — exercise bike interface.
        ; Used by "RacerMate Challenge" hardware/software.
        racermate_bicycle = $2c
        ; @brief Sharp C1 Cassette Interface (tape I/O interface for JP setups).
        sharp_cassette = $30
        ; @brief City Patrolman Lightgun (TimeTop; clone lightgun).
        timetop_lightgun = $2f
        ; @brief Miracle Piano (Antex for The Software Toolworks)
        ; 49-key MIDI keyboard/teaching system. Used by "The Miracle Piano 
        ; Teaching System" (NES cartridge)
        toolworks_miraclepiano = $19
        ; @brief TV 麻雀 Game (TV Mahjong Game) Controller
        ; Mahjong plug & play produced for the Japanese market; appears to 
        ; play a bootleg hack of Capcom's Ide Yousuke Meijin no Jissen Mahjong
        touchgameplayer_mahjong = $4b
        ; @brief Top Rider (Varie)
        ; Inflatable “bike” controller with steering and sensors. Used-by 
        ; "Top Rider" (Famicom; bundled accessory).
        varie_toprider = $1b
        ; @brief Venom TV Dance Mat (plug-and-play 4-in-1 dance pad).
        venom_dancemat = $39
        ; @brief Yonezawa Party Tap (six “buzzer” panels for quiz-style games).
        ; Used by "Gimme A Break! Shijō Saikyō no Quiz-Ō Ketteisen" and others.
        yonezawa_partytap = $16

        ; @brief Other alternative famiclone keyboard and mouse controllers
        .scope clonekbm
            ; @brief Puze / Bit Corp. (普澤 / Bit-79) keyboard
            bitcorp_keyboard = $25
            ; @brief Bùbùgāo (步步高, BBK) keyboard + R90-rotated PS/2 mouse via
            ; $4017.
            bubugao_keyboard_mouse_4017 = $46
            ; @brief Dongda (东达) PEC Keyboard
            dongda_keyboard = $24
            ; @brief Kēdá (科达) keyboard
            keda_keyboard = $35
            ; @brief Kēwáng (科王, Kingwon) keyboard.
            kewang_keyboard = $3e
            ; @brief PS/2 keyboard in UM6578 PS/2 port + PS/2 mouse via $4017.
            UMC_keyboard_mouse_4017 = $41
            ; @brief PS/2 mouse in UM6578 PS/2 port.
            UMC_mouse = $42
            ; @brief Yùxìng (裕兴) mouse via $4016.
            yuxing_mouse_4016 = $43
            ; @brief Zéchéng (泽诚) keyboard.
            zecheng_keyboard = $3f

            ; @brief SUBOR manufactured controllers
            .scope SUBOR
                ; @brief Subor (小霸王) keyboard (typical Famiclone keyboard).
                keyboard = $26
                ; @brief Subor keyboard + Macro Winners mouse (bundle).
                keyboard_mouse = $27
                ; @brief Subor keyboard + Subor mouse via $4016.
                keyboard_mouse_4016 = $28
                ; @brief Subor keyboard + Subor mouse via $4017.
                keyboard_mouse_4017 = $36
                ; @brief Subor keyboard + PS/2 mouse (L90-rotated) via $4017.
                ; “L90 rotated” indicates axes rotated by 90° in the mouse 
                ; report.
                keyboard_mousePS2_4017 = $40
                ; @brief Subor keyboard + X-inverted PS/2 mouse via $4017.
                ; @note X-axis is inverted in the mouse report.
                keyboard_mousePS2inv_4017 = $4d
                ; @brief Subor keyboard + Yuxing (裕兴) mouse via $4016.
                keyboard_mouseyux_4016 = $44
            .endscope
        .endscope
    .endscope
.endscope

.endif
