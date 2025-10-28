#ifndef NSK_ROMTYPE_ALTERNATIVE
#define NSK_ROMTYPE_ALTERNATIVE

/*!
 * \brief  List of nametables alternative possibilities
 *
 * \note Constructed from xconst_alternative file via XMacro
 */
enum nsk_alternative_type {

#   define X(key, name, value, description) \
    NSK_ALTERNATIVE_ ## name = value,

#       include <xconstants/nsk_xconst_alternative.x>

#   undef X

};

/*!
 * \brief  Returns text description of the type
 *
 * \param[in] type  The alternative type
 * \return Static string
 */
const char *nsk_alternative_name(enum nsk_alternative_type type);

#endif
