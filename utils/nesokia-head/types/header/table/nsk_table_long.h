#ifndef NSK_TABLE_LONG
#define NSK_TABLE_LONG

/*!
 * \brief  Converts bool into long description
 *
 * \param[in]  field  The field (`bool *` expected)
 * \return Static string
 */
const char *nsk_convlong_bool(const void *field);

/*!
 * \brief  Converts size in bytes into long description
 *
 * \param[in]  field  The field (`uint64_t *` expected)
 * \return Static string
 */
const char *nsk_convlong_bytes(const void *field);

/*!
 * \brief  Converts uint8_t into long description
 *
 * \param[in]  field  The field (`uint8_t *` expected)
 * \return Static string
 */
const char *nsk_convlong_u8(const void *field);

/*!
 * \brief  Converts uint16_t into long description
 *
 * \param[in]  field  The field (`uint16_t *` expected)
 * \return Static string
 */
const char *nsk_convlong_u16(const void *field);

/*!
 * Converts Mirroring mode into long description
 *
 * \param[in]  field  The field (`enum nsk_mirroring_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_mirroring(const void *field);

/*!
 * \brief  Converts console type into long description
 *
 * \param[in]  field  The field (`enum nsk_console_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_type(const void *field);

/*!
 * \brief  Converts region into long description
 *
 * \param[in]  field  The field (`enum nsk_region_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_region(const void *field);

/*!
 * \brief  Converts Vs. PPU type into long description
 *
 * \param[in]  field  The field (`enum nsk_vsppu_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_vsppu(const void *field);

/*!
 * \brief  Converts Vs. Hardware type into long description
 *
 * \param[in]  field  The field (`enum nsk_vshardware_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_vshardware(const void *field);

/*!
 * \brief  Converts expansion device into long description
 *
 * \param[in]  field  The field (`enum nsk_device_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_device(const void *field);

#endif