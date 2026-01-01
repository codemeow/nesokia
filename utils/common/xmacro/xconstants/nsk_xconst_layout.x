// X-Macro records for the nametables layout

// Metadata:
// [ Header: "Nametables layout" ]
// [ Scope: "NM_LAYOUT" ]
// [ Columns: 4 ]

// For languages that do not allow enum members to share the same value,
// you can include the key as part of the encoded value, for example:
// `name = key << 8 | value`.
// The getter function should then extract the original value using:
// `raw = input & 0xff`.

// key       name             value      description
X(  0, VERTICAL_ARRANGEMENT,    0, "Vertical arrangement"   )
X(  1, HORIZONTAL_ARRANGEMENT,  1, "Horizontal arrangment"  )
X(  2, VERTICAL_MIRRORING,      1, "Vertical mirroring"     )
X(  3, HORIZONTAL_MIRRORING,    0, "Horizontal mirroring"   )
X(  4, MAPPER_CONTROLLED,       0, "Mapper controlled"      )