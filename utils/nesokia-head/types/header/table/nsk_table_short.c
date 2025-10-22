#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include "../../../types/header/table/nsk_table_short.h"

#include "../../../types/header/enums/nsk_enum_console.h"
#include "../../../types/header/enums/nsk_enum_device.h"
#include "../../../types/header/enums/nsk_enum_mirroring.h"
#include "../../../types/header/enums/nsk_enum_region.h"
#include "../../../types/header/enums/nsk_enum_vshardware.h"
#include "../../../types/header/enums/nsk_enum_vsppu.h"

/*!
 * \brief  Direct uint64_t to string converter
 *
 * \param[in] value  The value
 * \return Static string
 */
static const char *_convert_u64(uint64_t value) {
    static char result[128];
    snprintf(result, sizeof(result), "%" PRIu64, value);
    return result;
}

/*!
 * \brief Converts bool into shortdescription
 *
 * \param[in] field  The field (`bool *` expected)
 * \return Static string
 */
const char *nsk_convshort_bool(const void *field) {
    bool value = *(bool *)field;
    return _convert_u64(value);
}

/*!
 * \brief  Converts uint64_t into short description
 *
 * \param[in] field  The field (`uint64_t *` expected)
 * \return Static string
 */
const char *nsk_convshort_u64(const void *field) {
    uint64_t value = *(uint64_t *)field;
    return _convert_u64(value);
}

/*!
 * \brief  Converts uint8_t into short description
 *
 * \param[in] field  The field (`uint8_t *` expected)
 * \return Static string
 */
const char *nsk_convshort_u8(const void *field) {
    uint8_t value = *(uint8_t *)field;
    return _convert_u64(value);
}

/*!
 * \brief  Converts uint16_t into short description
 *
 * \param[in] field  The field (`uint16_t *` expected)
 * \return Static string
 */
const char *nsk_convshort_u16(const void *field) {
    uint16_t value = *(uint16_t *)field;
    return _convert_u64(value);
}


/*!
 * Converts Mirroring mode into short description
 *
 * \param[in]  field  The field (`enum nsk_mirroring_type *` expected)
 * \return Static string
 */
const char *nsk_convshort_mirroring(const void *field) {
    enum nsk_mirroring_type value = *(enum nsk_mirroring_type *)field;
    return _convert_u64(value);
}

/*!
 * \brief  Converts console type into short description
 *
 * \param[in] field  The field (`enum nsk_console_type *` expected)
 * \return Static string
 */
const char *nsk_convshort_type(const void *field) {
    enum nsk_console_type value = *(enum nsk_console_type *)field;
    return _convert_u64(value);
}

/*!
 * \brief  Converts console region into short description
 *
 * \param[in] field  The field (`enum nsk_region_type *` expected)
 * \return Static string
 */
const char *nsk_convshort_region(const void *field) {
    enum nsk_region_type value = *(enum nsk_region_type *)field;
    return _convert_u64(value);
}

/*!
 * \brief  Converts Vs. PPU type into short description
 *
 * \param[in] field  The field (`enum nsk_vsppu_type *` expected)
 * \return Static string
 */
const char *nsk_convshort_vsppu(const void *field) {
    enum nsk_vsppu_type value = *(enum nsk_vsppu_type *)field;
    return _convert_u64(value);
}

/*!
 * \brief  Converts Vs. Hardware type into short description
 *
 * \param[in] field  The field (`enum nsk_vshardware_type *` expected)
 * \return Static string
 */
const char *nsk_convshort_vshardware(const void *field) {
    enum nsk_vshardware_type value = *(enum nsk_vshardware_type *)field;
    return _convert_u64(value);
}

/*!
 * \brief  Converts expansion device type into short description
 *
 * \param[in] field  The field (`nsk_device_type *` expected)
 * \return Static string
 */
const char *nsk_convshort_device(const void *field) {
    enum nsk_device_type value = *(enum nsk_device_type *)field;
    return _convert_u64(value);
}
