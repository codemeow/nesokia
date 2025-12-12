#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#include "../../../types/header/table/nsk_table_long.h"

#include "../../../types/header/enums/nsk_enum_alternative.h"
#include "../../../types/header/enums/nsk_enum_battery.h"
#include "../../../types/header/enums/nsk_enum_console.h"
#include "../../../types/header/enums/nsk_enum_device.h"
#include "../../../types/header/enums/nsk_enum_mirroring.h"
#include "../../../types/header/enums/nsk_enum_region.h"
#include "../../../types/header/enums/nsk_enum_trainer.h"
#include "../../../types/header/enums/nsk_enum_vshardware.h"
#include "../../../types/header/enums/nsk_enum_vsppu.h"
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
 * Converts Mirroring mode into long description
 *
 * \param[in]  field  The field (`enum nsk_alternative_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_alternative(const void *field) {
    enum nsk_alternative_type value = *(enum nsk_alternative_type *)field;
    return nsk_alternative_name(value);
}

/*!
 * Converts Mirroring mode into long description
 *
 * \param[in]  field  The field (`enum nsk_battery_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_battery(const void *field) {
    enum nsk_battery_type value = *(enum nsk_battery_type *)field;
    return nsk_battery_name(value);
}

/*!
 * Converts Mirroring mode into long description
 *
 * \param[in]  field  The field (`enum nsk_trainer_type *` expected)
 * \return Static string
 */
const char *nsk_convlong_trainer(const void *field) {
    enum nsk_trainer_type value = *(enum nsk_trainer_type *)field;
    return nsk_trainer_name(value);
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
