#pragma once

/*!
 * \brief Attribute to apply the warning if the result
 * of the function is not used
 */
#define nsk_attr_result_unused __attribute__((warn_unused_result))

/*!
 * \brief Attribute to apply the non-null checking for
 * the function arguments
 * \param[in] ... list of argument indexes, starting with 1
 */
#define nsk_attr_args_nonnull(...) __attribute__((nonnull(__VA_ARGS__)))