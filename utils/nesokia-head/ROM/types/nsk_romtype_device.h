#ifndef NSK_ROMTYPE_DEVICE
#define NSK_ROMTYPE_DEVICE

/*!
 * \brief  List of expansion devices
 *
 * \note Constructed from xconst_vsppu file via XMacro,
 * expected format:
 * ~~~
 * NSK_NONE_UNSPECIFIED      = 0,
 * NSK_NONE_MULTICART        = 42,
 * ...
 * ~~~
 */
enum nsk_device_type {

#   define X(key, manufacturer, device, value, description) \
    NSK_DEVICE_ ## manufacturer ## _ ## device = value,

#       include "../../../xconstants/nsk_xconst_device.x"

#   undef X

};

/*!
 * \brief  Returns string description of the expansion device
 *
 * \param[in] type  The type
 * \return  Static string
 */
const char *nsk_device_name(enum nsk_device_type type);

#endif