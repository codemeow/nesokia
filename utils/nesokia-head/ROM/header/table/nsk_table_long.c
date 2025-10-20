#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#include "../../../ROM/header/table/nsk_table_long.h"
#include "../../../ROM/types/nsk_romtype_console.h"
#include "../../../ROM/types/nsk_romtype_device.h"
#include "../../../ROM/types/nsk_romtype_region.h"
#include "../../../ROM/types/nsk_romtype_vshardware.h"
#include "../../../ROM/types/nsk_romtype_vsppu.h"
#include "../../../ROM/types/nsk_romtype_mirroring.h"
#include "../../../utils/nsk_util_hrbytes.h"

/*!
 * \brief  Converts bool into long description
 *
 * \param[in]  field  The field (`bool *` expected)
 * \return Static string
 */
const char *nsk_convlong_bool(const void *field) {
    const bool value = *(bool *)field;
    return value ? "Yes" : "No";
}

/*!
 * \brief  Converts size in bytes into long description
 *
 * \param[in]  field  The field (`uint64_t *` expected)
 * \return Static string
 */
const char *nsk_convlong_bytes(const void *field) {
    const uint64_t value = *(uint64_t *)field;
    return nsk_util_hrbytes(value);
}

/*!
 * \brief  Converts uint8_t into long description
 *
 * \param[in]  field  The field (`uint8_t *` expected)
 * \return Static string
 */
const char *nsk_convlong_u8(const void *field) {
    static char result[128];
    const uint8_t value = *(uint8_t *)field;

    snprintf(result, sizeof(result), "%" PRIu8, value);
    return result;
}

/*!
 * \brief  Converts uint16_t into long description
 *
 * \param[in]  field  The field (`uint16_t *` expected)
 * \return Static string
 */
const char *nsk_convlong_u16(const void *field) {
    static char result[128];
    const uint16_t value = *(uint16_t *)field;

    snprintf(result, sizeof(result), "%" PRIu16, value);
    return result;
}

/*!
 * Converts Mirroring mode into long description
 *
 * \param[in]  field  The field (`enum nsk_mirroring_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_mirroring(const void *field) {
    enum nsk_mirroring_type value = *(enum nsk_mirroring_type *)field;
    return nsk_mirroring_name(value);
}

/*!
 * \brief  Converts console type into long description
 *
 * \param[in]  field  The field (`enum nsk_console_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_type(const void *field) {
    const enum nsk_console_type value = *(enum nsk_console_type *)field;
    return nsk_console_name(value);
}

/*!
 * \brief  Converts region into long description
 *
 * \param[in]  field  The field (`enum nsk_region_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_region(const void *field) {
    const enum nsk_region_type value = *(enum nsk_region_type *)field;
    return nsk_region_name(value);
}

/*!
 * \brief  Converts Vs. PPU type into long description
 *
 * \param[in]  field  The field (`enum nsk_vsppu_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_vsppu(const void *field) {
    const enum nsk_vsppu_type value = *(enum nsk_vsppu_type *)field;
    return nsk_vsppu_name(value);
}

/*!
 * \brief  Converts Vs. Hardware type into long description
 *
 * \param[in]  field  The field (`enum nsk_vshardware_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_vshardware(const void *field) {
    const enum nsk_vshardware_type value = *(enum nsk_vshardware_type *)field;
    return nsk_vshardware_name(value);
}

/*!
 * \brief  Converts expansion device into long description
 *
 * \param[in]  field  The field (`enum nsk_device_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_device(const void *field) {
    const enum nsk_device_type value = *(enum nsk_device_type *)field;
    return nsk_device_name(value);
}
