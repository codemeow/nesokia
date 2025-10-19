#ifndef NSK_ROMTYPE_VSHARDWARE
#define NSK_ROMTYPE_VSHARDWARE

/*!
 * \brief  List of Vs Systems hardware types
 *
 * \note Constructed from xconst_vsppu file via XMacro,
 * expected format:
 * ~~~
 * NSK_VSHARDWARE_VSUNI_NORMAL      = 0,
 * NSK_VSHARDWARE_VSUNI_BASEBALL    = 1,
 * ...
 * ~~~
 */
enum nsk_vshardware_type {

#   define X(key, name, value, description) \
    NSK_VSHARDWARE_ ## name = value,

#       include "../../../xconstants/nsk_xconst_vshardware.x"

#   undef X

};

/*!
 * \brief  Returns string description of the Vs.hardware
 *
 * \param[in] type  The type
 * \return  Static string
 */
const char *nsk_vshardware_name(enum nsk_vshardware_type type);

#endif