// X-Macro records for the console region

// For languages that do not allow enum members to share the same value,
// you can include the key as part of the encoded value, for example:
// `name = key << 8 | value`.
// The getter function should then extract the original value using:
// `raw = input & 0xff`.

// key       name      value        description
X(  0, RP2C02,           0, "RP2C02 PPU"                    )
X(  1, NTSC,             0, "NTSC 21.47 MHz"                )
X(  2, NORTH_AMERICA,    0, "North America"                 )
X(  3, JAPAN,            0, "Japan"                         )
X(  4, SOUTH_KOREA,      0, "South Korea"                   )
X(  5, TAIWAN,           0, "Taiwan"                        )

X(  6, RP2C07,           1, "RP2C07 PPU"                    )
X(  7, PAL,              1, "PAL 26.60 MHz"                 )
X(  8, WESTERN_EUROPE,   1, "West. Europe"                  )
X(  9, AUSTRALIA,        1, "Australia"                     )

X( 10, MULTIPLE,         2, "Multiple regions/modern"       )

X( 11, UA6538,           3, "NTSC/PAL hybrid"               )
X( 12, DENDY,            3, "Dendy"                         )
X( 13, EASTERN_EUROPE,   3, "East. Europe"                  )
X( 14, RUSSIA,           3, "Russia"                        )
X( 15, MAINLAND_CHINA,   3, "China"                         )
X( 16, INDIA,            3, "India"                         )
X( 17, AFRICA,           3, "Africa"                        )
