// X-Macro records for the Vs System PPU type

// For languages that do not allow enum members to share the same value,
// you can include the key as part of the encoded value, for example:
// `name = key << 8 | value`.
// The getter function should then extract the original value using:
// `raw = input & 0xff`.

// key       name         value            description
X(  0, NONE,               -1, "---"                        )
X(  1, RP2C03,              0, "Any RP2C03"                 )
X(  2, RC2C03,              0, "Any RC2C03"                 )
X(  3, RP2C04_001,          2, "RP2C04-001"                 )
X(  4, RP2C04_002,          3, "RP2C04-002"                 )
X(  5, RP2C04_003,          4, "RP2C04-003"                 )
X(  6, RP2C04_004,          5, "RP2C04-004"                 )
X(  7, RC2C05_01,           8, "RC2C05-01"                  )
X(  8, RC2C05_02,           9, "RC2C05-02"                  )
X(  9, RC2C05_03,          10, "RC2C05-03"                  )
X( 10, RC2C05_04,          11, "RC2C05-04"                  )
