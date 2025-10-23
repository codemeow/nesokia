#ifndef NSK_ROMTYPE_VSPPU
#define NSK_ROMTYPE_VSPPU

/*!
 * \brief  List of Vs Systems PPU types
 *
 * \note Constructed from xconst_vsppu file via XMacro,
 * expected format:
 * ~~~
 * NSK_VSPPU_RP2C03  = 0,
 * NSK_VSPPU_RC2C03  = 0,
 * ...
 * ~~~
 */
enum nsk_vsppu_type {

#   define X(key, name, value, description) \
    NSK_VSPPU_ ## name = value,

#       include <xconstants/nsk_xconst_vsppu.x>

#   undef X

};

/*!
 * \brief  Returns string description of the Vs. PPU
 *
 * \param[in] type  The type
 * \return  Static string
 */
const char *nsk_vsppu_name(enum nsk_vsppu_type type);

#endif