#ifndef NSK_TABLE_CMP
#define NSK_TABLE_CMP

#include "../../../ROM/header/nsk_header_table.h"

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

#endif
