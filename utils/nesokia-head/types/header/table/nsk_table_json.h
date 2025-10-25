#ifndef NSK_TABLE_JSON
#define NSK_TABLE_JSON

/*!
 * \brief  Converts bool into JSON format
 *
 * \param[in]  field  The field (`bool *` expected)
 * \return Static string
 */
const char *nsk_convjson_bool(const void *field);

/*!
 * \brief  Converts uint64_t into JSON format
 *
 * \param[in]  field  The field (`uint64_t *` expected)
 * \return Static string
 */
const char *nsk_convjson_u64(const void *field);

/*!
 * \brief  Converts uint8_t into JSON format
 *
 * \param[in]  field  The field (`uint8_t *` expected)
 * \return Static string
 */
const char *nsk_convjson_u8(const void *field);

/*!
 * \brief  Converts uint16_t into JSON format
 *
 * \param[in]  field  The field (`uint16_t *` expected)
 * \return Static string
 */
const char *nsk_convjson_u16(const void *field);

/*!
 * Converts Mirroring mode into JSON format
 *
 * \param[in]  field  The field (`enum nsk_mirroring_type *` expected)
 * \return Static string
 */
const char *nsk_convjson_mirroring(const void *field);

/*!
 * \brief  Converts console type into JSON format
 *
 * \param[in]  field  The field (`enum nsk_console_type *` expected)
 * \return Static string
 */
const char *nsk_convjson_type(const void *field);

/*!
 * \brief  Converts region into JSON format
 *
 * \param[in]  field  The field (`enum nsk_region_type *` expected)
 * \return Static string
 */
const char *nsk_convjson_region(const void *field);

/*!
 * \brief  Converts Vs. PPU type into JSON format
 *
 * \param[in]  field  The field (`enum nsk_vsppu_type *` expected)
 * \return Static string
 */
const char *nsk_convjson_vsppu(const void *field);

/*!
 * \brief  Converts Vs. Hardware type into JSON format
 *
 * \param[in]  field  The field (`enum nsk_vshardware_type *` expected)
 * \return Static string
 */
const char *nsk_convjson_vshardware(const void *field);

/*!
 * \brief  Converts expansion device into JSON format
 *
 * \param[in]  field  The field (`enum nsk_device_type *` expected)
 * \return Static string
 */
const char *nsk_convjson_device(const void *field);

#endif