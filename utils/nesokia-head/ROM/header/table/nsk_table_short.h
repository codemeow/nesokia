#ifndef NSK_TABLE_SHORT
#define NSK_TABLE_SHORT

/*!
 * \brief Converts bool into shortdescription
 *
 * \param[in] field  The field (`bool *` expected)
 * \return Static string
 */
const char *nsk_convshort_bool(const void *field);

/*!
 * \brief  Converts uint64_t into short description
 *
 * \param[in] field  The field (`uint64_t *` expected)
 * \return Static string
 */
const char *nsk_convshort_u64(const void *field);

/*!
 * \brief  Converts uint8_t into short description
 *
 * \param[in] field  The field (`uint8_t *` expected)
 * \return Static string
 */
const char *nsk_convshort_u8(const void *field);

/*!
 * \brief  Converts uint16_t into short description
 *
 * \param[in] field  The field (`uint16_t *` expected)
 * \return Static string
 */
const char *nsk_convshort_u16(const void *field);

/*!
 * \brief  Converts console type into short description
 *
 * \param[in] field  The field (`enum nsk_console_type *` expected)
 * \return Static string
 */
const char *nsk_convshort_type(const void *field);

/*!
 * \brief  Converts console region into short description
 *
 * \param[in] field  The field (`enum nsk_region_type *` expected)
 * \return Static string
 */
const char *nsk_convshort_region(const void *field);

/*!
 * \brief  Converts Vs. PPU type into short description
 *
 * \param[in] field  The field (`enum nsk_vsppu_type *` expected)
 * \return Static string
 */
const char *nsk_convshort_vsppu(const void *field);

/*!
 * \brief  Converts Vs. Hardware type into short description
 *
 * \param[in] field  The field (`enum nsk_vshardware_type *` expected)
 * \return Static string
 */
const char *nsk_convshort_vshardware(const void *field);

/*!
 * \brief  Converts expansion device type into short description
 *
 * \param[in] field  The field (`nsk_device_type *` expected)
 * \return Static string
 */
const char *nsk_convshort_device(const void *field);

#endif