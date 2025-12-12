#ifndef NSK_ROMTYPE_BATTERY
#define NSK_ROMTYPE_BATTERY

/*!
 * \brief  List of battery presence possibilities
 *
 * \note Constructed from xconst_battery file via XMacro
 */
enum nsk_battery_type {

#   define X(key, name, value, description) \
    NSK_BATTERY_ ## name = value,

#       include <xconstants/nsk_xconst_battery.x>

#   undef X

};

/*!
 * \brief  Returns text description of the type
 *
 * \param[in] type  The battery type
 * \return Static string
 */
const char *nsk_battery_name(enum nsk_battery_type type);

#endif
