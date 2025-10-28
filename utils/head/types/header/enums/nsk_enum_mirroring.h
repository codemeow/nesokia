#ifndef NSK_ROMTYPE_MIRRORING
#define NSK_ROMTYPE_MIRRORING

/*!
 * \brief  List of console regions/timings
 *
 * \note Constructed from xconst_mirroring file via XMacro
 */
enum nsk_mirroring_type {

#   define X(key, name, value, description) \
    NSK_MIRRORING_ ## name = value,

#       include <xconstants/nsk_xconst_layout.x>

#   undef X

};

/*!
 * \brief  Returns text description of the type
 *
 * \param[in] type  The mirroring type
 * \return Static string
 */
const char *nsk_mirroring_name(enum nsk_mirroring_type type);

#endif