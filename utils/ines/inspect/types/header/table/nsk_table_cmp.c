#include <stdint.h>

#include "../../../types/header/table/nsk_table_cmp.h"
#include "../../../types/header/enums/nsk_enum_alternative.h"
#include "../../../types/header/enums/nsk_enum_battery.h"
#include "../../../types/header/enums/nsk_enum_console.h"
#include "../../../types/header/enums/nsk_enum_device.h"
#include "../../../types/header/enums/nsk_enum_mirroring.h"
#include "../../../types/header/enums/nsk_enum_region.h"
#include "../../../types/header/enums/nsk_enum_trainer.h"
#include "../../../types/header/enums/nsk_enum_vshardware.h"
#include "../../../types/header/enums/nsk_enum_vsppu.h"

/*!
 * \brief  Directly compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
static bool _compare_u64(
    uint64_t field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    return nsk_pair_compare(field, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_bool(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    bool fvalue = *(bool *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_u64(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    uint64_t fvalue = *(uint64_t *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_u8(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    uint8_t fvalue = *(uint8_t *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_u16(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    uint16_t fvalue = *(uint16_t *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_mirroring(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    enum nsk_mirroring_type fvalue = *(enum nsk_mirroring_type *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_alternative(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    enum nsk_alternative_type fvalue = *(enum nsk_alternative_type *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_battery(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    enum nsk_battery_type fvalue = *(enum nsk_battery_type *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_trainer(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    enum nsk_trainer_type fvalue = *(enum nsk_trainer_type *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_type(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    enum nsk_console_type fvalue = *(enum nsk_console_type *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_region(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    enum nsk_region_type fvalue = *(enum nsk_region_type *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_vsppu(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    enum nsk_vsppu_type fvalue = *(enum nsk_vsppu_type *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_vshardware(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    enum nsk_vshardware_type fvalue = *(enum nsk_vshardware_type *)field;
    return _compare_u64(fvalue, value, operator);
}

/*!
 * \brief  Compares the field value with the reference value
 *
 * Answer the question "is <field> <operator> <value>?", i.e.
 * "Is mapper > 304?"
 *
 * \param[in] field      The field
 * \param[in] value      The value
 * \param[in] operator   The required operator
 * \return True if <field> <operator> <value>
 */
bool nsk_compare_device(
    const void *field,
    uint64_t value,
    enum nsk_pair_operator operator
) {
    enum nsk_device_type fvalue = *(enum nsk_device_type *)field;
    return _compare_u64(fvalue, value, operator);
}
