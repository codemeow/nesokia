// X-Macro records for the external device type

// Metadata:
// [ Header: "Expansion devices list" ]
// [ Scope: "DEVICES" ]
// [ Columns: 6 ]

// General
// key   manufacturer         device               value                  description
X(  0, NONE,            UNSPECIFIED,                 0, "Unspecified/none"                                       )
X(  1, NONE,            MULTICART,                  42, "Multicart selector"                                     )

// Famicom/NES accessories
// key   manufacturer         device               value                  description
X(  2, NINTENDO,        CONTROLLERS,                 1, "Standard Famicom/NES controllers"                       )
X(  3, NINTENDO,        CONTROLLERS_INV,            49, "Famicom/NES controller with swapped LR/UD/BA"           )
X(  4, NINTENDO,        CONTROLLERS_TEST,           55, "Famicom/NES port test controller"                       )
X(  5, NINTENDO,        FOURPLAYERS_SIMPLE,          3, "Famicom Four Players adapter (simple protocol)"         )
X(  6, NINTENDO,        KEYBOARD_NOINPUT_DATAREC,   32, "Famicom BASIC keyboard with no input and Data Recorder" )
X(  7, NINTENDO,        KEYBOARD_BASIC_DATAREC,     35, "Famicom BASIC keyboard and Data Recorder"               )
X(  8, NINTENDO,        CONTROLLER_NETWORK,         59, "Famicom/NES Network Controller"                         )
X(  9, NINTENDO,        ROB_GYRO,                   31, "Famicom/NES R.O.B. Gyro set"                            )
X( 10, NINTENDO,        ROB_STACKUP,                46, "Famicom/NES R.O.B. Stack-Up set"                        )
X( 11, NINTENDO,        ZAPPER_4016,                73, "NES Zapper on port $4016"                               )
X( 12, NINTENDO,        ZAPPER_4017,                 8, "Famicom/NES Zapper on port $4017"                       )
X( 13, NINTENDO,        ZAPPER_DUAL,                 9, "NES dual Zappers"                                       )
X( 14, NINTENDO,        SYSTEM3D,                   29, "Famicom 3D System"                                      )
X( 15, NINTENDO,        FOURSCORE,                   2, "NES Four Score/Satellite"                               )
X( 16, NINTENDO,        POWERPAD_A,                 11, "Famicom/NES PowerPad (Side A)"                          )
X( 17, NINTENDO,        POWERPAD_B,                 12, "Famicom/NES PowerPad (Side B)"                          )
X( 18, NINTENDO,        VAUS,                       15, "NES Arkanoid Vaus paddle"                               )

// Vs. System accessories
// key   manufacturer         device               value                  description
X( 19, VSSYSTEM,        4016,                        4, "Vs. System, 1P via $4016"                               )
X( 20, VSSYSTEM,        4017,                        5, "Vs. System, 1P via $4017"                               )
X( 21, VSSYSTEM,        ZAPPER,                      7, "Vs. System zapper/lightgun"                             )

// Bandai accessories
// key   manufacturer         device               value                  description
X( 22, BANDAI,          COOKINGPLAYSET,             71, "Bandai Magical Cooking playset"                         )
X( 23, BANDAI,          FAMILYTRAINER_A,            13, "Bandai Family Trainer (Side A)"                         )
X( 24, BANDAI,          FAMILYTRAINER_B,            14, "Bandai Family Trainer (Side B)"                         )
X( 25, BANDAI,          GAMEPAD,                    56, "Bandai Multi Game gamepad extra buttons"                )
X( 26, BANDAI,          HYPERSHOT,                  10, "Bandai Hypershot lightgun"                              )
X( 27, BANDAI,          TABLET,                     23, "Bandai Oeka Kids Tablet"                                )

// Konami accessories
// key   manufacturer         device               value                  description
X( 28, KONAMI,          DOREMIKKO,                  30, "Konami DoReMiKko music keayboard"                       )
X( 29, KONAMI,          HYPERSHOT,                  18, "Konami Dual trigger controller"                         )
X( 30, KONAMI,          PUNCHINGBAG,                20, "Konami Exciting Boxing punching bag"                    )

// IGS accessories
// key   manufacturer         device               value                  description
X( 31, IGS,             BATTLEBOX,                  34, "IGS Battle Box"                                         )
X( 32, IGS,             POKKUNMOGURA,               26, "IGS Pokkun Moguraa Whack-a-mole"                        )

// SNES accessories
// key   manufacturer         device               value                  description
X( 33, SNES,            CONTROLLERS,                43, "SNES Controllers"                                       )
X( 34, SNES,            MOUSE_4016,                 41, "SNES Mouse via $4016"                                   )
X( 35, SNES,            MOUSE_4017,                 72, "SNES Mouse via $4017"                                   )

// Taito accessories
// key   manufacturer         device               value                  description
X( 36, TAITO,           VAUS,                       16, "Taito Arkanoid Vaus paddle"                             )
X( 37, TAITO,           VAUS_PROTO,                 74, "Taito Arkanoid Vaus paddle (prototype version)"         )
X( 38, TAITO,           VAUS_DUAL_DATAREC,          17, "Taito dual Arkanoid vaus paddles with Data Recorder"    )

// Others
// key   manufacturer         device               value                  description
X( 39, ABL,             PINBALL,                    51, "ABL Pinball — extra pinball buttons controller"         )
X( 40, ADVANCEDBRIGHT,  CROAKYKARAOKE,              61, "Advanced Bright Croaky Karaoke"                         )
X( 41, ASCII,           TURBOFILE,                  33, "ASCII Turbo File storage"                               )
X( 42, BRODERBUND,      UFORCE,                     45, "Brøderbund U-Force controller"                          )
X( 43, CAPCOM,          MAHJONG,                    21, "Capcom 21-button mahjong panel"                         )
X( 44, COCONUTS,        PACHINKO,                   19, "Coconuts pachinko controller"                           )
X( 45, ELECOM,          MAHJONG,                    76, "Elecom Mahjong Gekitou Densetsu controller"             )
X( 46, EPOCH,           BARCODEBATTLER2,            24, "Epoch Barcode Battler II interface"                     )
X( 47, EXCALIBUR,       SUDOKU,                     50, "Excalibur Sudoku Pad"                                   )
X( 48, GIGGGLE,         PUMP,                       69, "Gigggle TV Pump"                                        )
X( 49, HUMMER,          FISHINGROD,                 60, "Hummer King Fishing Controller"                         )
X( 50, IBM,             KEYBOARD,                   78, "IBM PC/XT keyboard"                                     )
X( 51, LG,              TVREMOTE,                   58, "LG TV Remote"                                           )
X( 52, MAJESCO,         CASINOCONTROLLER,           52, "Majesco Golden Nugget Casino extra buttons"             )
X( 53, NICHIBUTSU,      DOUBLE,                     28, "Nichibutsu double controllers (double fisted play)"     )
X( 54, RACERMATE,       BICYCLE,                    44, "RacerMate Bicycle"                                      )
X( 55, SHARP,           CASSETTE,                   48, "Sharp C1 Cassette Interface"                            )
X( 56, TIMETOP,         LIGHTGUN,                   47, "Timetop City Patrolman Lightgun"                        )
X( 57, TOOLWORKS,       MIRACLEPIANO,               25, "Toolworks Miracle Piano"                                )
X( 58, TOUCHGAMEPLAYER, MAHJONG,                    75, "TouchgamePlayer TV 麻雀 Game"                             )
X( 59, VARIE,           TOPRIDER,                   27, "Varie Inflatable bike"                                  )
X( 60, VENOM,           DANCEMAT,                   57, "Venom TV Dance Mat"                                     )
X( 61, YONEZAWA,        PARTYTAP,                   22, "Yonezawa Party Tap"                                     )

// Famiclones keyboard/mouse sets
// key   manufacturer         device               value                  description
X( 62, BITCORP,         KEYBOARD,                   37, "Bitcorp (普澤 / Bit-79) keyboard"                         )
X( 63, BUBUGAO,         KEYBOARD_MOUSE_4017,        70, "Bùbùgāo (步步高, BBK) keyboard with PS/2 mouse via $4017"  )
X( 64, DONGDA,          KEYBOARD,                   36, "Dongda (东达) PEC Keyboard"                               )
X( 65, KEDA,            KEYBOARD,                   53, "Kēdá (科达) keyboard"                                     )
X( 66, KEWANG,          KEYBOARD,                   62, "Kēwáng (科王, Kingwon) keyboard"                          )
X( 67, UMC,             KEYBOARD_MOUSE_4017,        65, "UMC PS/2 keyboard wth PS/2 mouse via $4017"             )
X( 68, UMC,             MOUSE,                      66, "UMC PS/2 mouse"                                         )
X( 69, YUXING,          MOUSE_4016,                 67, "Yùxìng (裕兴) mouse via $4016"                            )
X( 70, ZECHENG,         KEYBOARD,                   63, "Zéchéng (泽诚) keyboard"                                  )

// SUBOR keyboard/mouse sets
// key   manufacturer         device               value                  description
X( 71, SUBOR,           KEYBOARD,                   38, "Subor (小霸王) keyboard"                                   )
X( 72, SUBOR,           KEYBOARD_MOUSE,             39, "Subor (小霸王) keyboard + Macro Winners mouse"             )
X( 73, SUBOR,           KEYBOARD_MOUSE_4016,        40, "Subor (小霸王) keyboard + Subor mouse via $4016"           )
X( 74, SUBOR,           KEYBOARD_MOUSE_4017,        54, "Subor (小霸王) keyboard + Subor mouse via $4017"           )
X( 75, SUBOR,           KEYBOARD_MOUSEPS2_4017,     64, "Subor (小霸王) keyboard + PS/2 mouse via $4017"            )
X( 76, SUBOR,           KEYBOARD_MOUSEPS2INV_4017,  77, "Subor (小霸王) keyboard + X-inverted PS/2 mouse via $4017" )
X( 77, SUBOR,           KEYBOARD_MOUSEYUX_4016,     68, "Subor (小霸王) keyboard + Yuxing (裕兴) mouse via $4016"     )
