// X-Macro records for the console type

// For languages that do not allow enum members to share the same value,
// you can include the key as part of the encoded value, for example:
// `name = key << 8 | value`.
// The getter function should then extract the original value using:
// `raw = input & 0xff`.

// key       name         value            description
X(  0, NES,                 0, "Nintendo Entertainment System"              )
X(  1, FAMICOM,             0, "Famicom"                                    )
X(  2, DENDY,               0, "Dendy"                                      )
X(  3, NINTENDO_VS,         1, "Nintendo Vs. System"                        )
X(  4, PLAYCHOICE10,        2, "Playchoice 10"                              )
X(  5, FAMICLONE_DEC,       3, "Famiclone with decial support"              )
X(  6, FAMICOM_EPSM,        4, "Famicom with EPSM or plug-through cartridge")
X(  7, VRTECHNOLOGY_VT01,   5, "V.R. Technology VT01"                       )
X(  8, VRTECHNOLOGY_VT02,   6, "V.R. Technology VT02"                       )
X(  9, VRTECHNOLOGY_VT03,   7, "V.R. Technology VT03"                       )
X( 10, VRTECHNOLOGY_VT09,   8, "V.R. Technology VT09"                       )
X( 11, VRTECHNOLOGY_VT32,   9, "V.R. Technology VT32"                       )
X( 12, VRTECHNOLOGY_VT369, 10, "V.R. Technology VT369"                      )
X( 13, UMC_UM6578,         11, "Single chip famiclone based on UM6578"      )
X( 14, FAMICOM_NETWORK,    12, "Famicom Network System"                     )