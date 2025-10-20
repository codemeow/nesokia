#include <stdlib.h>
#include <stdint.h>

#include "../../../ROM/header/table/nsk_table_cmp.h"
#include "../../../ROM/types/nsk_romtype_console.h"
#include "../../../ROM/types/nsk_romtype_device.h"
#include "../../../ROM/types/nsk_romtype_region.h"
#include "../../../ROM/types/nsk_romtype_vshardware.h"
#include "../../../ROM/types/nsk_romtype_vsppu.h"
#include "../../../ROM/types/nsk_romtype_mirroring.h"
#include "../../../output/nsk_output_err.h"

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
    enum nsk_match_operator operator
) {
    switch (operator) {
        case NSK_MATCH_EQUAL:       return field == value;
        case NSK_MATCH_NOTEQUAL:    return field != value;
        case NSK_MATCH_LESS:        return field  < value;
        case NSK_MATCH_LESSOREQUAL: return field <= value;
        case NSK_MATCH_MORE:        return field  > value;
        case NSK_MATCH_MOREOREQUAL: return field >= value;
    }

    nsk_err("Unknown operator: %d\n", (int)operator);
    abort();
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
    enum nsk_match_operator operator
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
    enum nsk_match_operator operator
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
    enum nsk_match_operator operator
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
    enum nsk_match_operator operator
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
    enum nsk_match_operator operator
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
bool nsk_compare_type(
    const void *field,
    uint64_t value,
    enum nsk_match_operator operator
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
    enum nsk_match_operator operator
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
    enum nsk_match_operator operator
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
    enum nsk_match_operator operator
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
    enum nsk_match_operator operator
) {
    enum nsk_device_type fvalue = *(enum nsk_device_type *)field;
    return _compare_u64(fvalue, value, operator);
}
