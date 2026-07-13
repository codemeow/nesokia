// X-Macro records for the Vs System hardware type

// Metadata:
// [ Header: "Vs. hardware types" ]
// [ Scope: "VSHARDWARE" ]
// [ Columns: 4 ]

// For languages that do not allow enum members to share the same value,
// you can include the key as part of the encoded value, for example:
// `name = key << 8 | value`.
// The getter function should then extract the original value using:
// `raw = input & 0xff`.

// key       name     value            description
X(  0, NONE,            0, "Not applicable"                                    )
X(  0, VSUNI_NORMAL,    0, "Vs. Unisystem (normal)"                            )
X(  1, VSUNI_BASEBALL,  1, "Vs. Unisystem (RBI Baseball protection)"           )
X(  2, VSUNI_BOXING,    2, "Vs. Unisystem (TKO Boxing protection)"             )
X(  3, VSUNI_XEVIOUS,   3, "Vs. Unisystem (Super Xevious protection)"          )
X(  4, VSUNI_CLIMBER,   4, "Vs. Unisystem (Jp Ice Climber protection)"         )
X(  5, VSDUAL_NORMAL,   5, "Vs. Dual System (normal)"                          )
X(  6, VSDUL_RAID,      6, "Vs. Dual System (Raid on Bungeling Bay protection)")