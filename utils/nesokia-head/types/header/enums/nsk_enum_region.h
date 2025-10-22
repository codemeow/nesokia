#ifndef NSK_ROMTYPE_REGION
#define NSK_ROMTYPE_REGION

/*!
 * \brief  List of console regions/timings
 *
 * \note Constructed from xconst_region file via XMacro,
 * expected format:
 * ~~~
 * NSK_REGION_RP2C02 = 0,
 * NSK_REGION_NTSC   = 0,
 * ...
 * ~~~
 */
enum nsk_region_type {

#   define X(key, name, value, description) \
    NSK_REGION_ ## name = value,

#       include <xconstants/nsk_xconst_region.x>

#   undef X

};

/*!
 * \brief  Returns string description of the region type
 *
 * \param[in] type  The type
 * \return  Static string
 */
const char *nsk_region_name(enum nsk_region_type type);

#endif