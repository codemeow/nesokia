#ifndef NSK_UTILS_MALLOC
#define NSK_UTILS_MALLOC

#include <stdlib.h>

#include "base/nsk_util_attributes.h"

/*!
 * \brief  Allocates the required amount of memory and fills it with zeroes
 *
 * \param[in] size  The size
 * \return Allocated memory
 *
 * \note This function never returns NULL. It either returns allocated memory
 *       or terminates the process.
 */
nsk_attr_result_unused
void *nsk_util_malloc(size_t size);

/*!
 * \brief  Reallocates the provided region
 *
 * \param[in] ptr   The pointer
 * \param[in] size  The size
 * \return Reallocated memory block
 *
 * \note This function never returns NULL. It either returns reallocated memory
 *       or terminates the process.
 */
nsk_attr_result_unused
void *nsk_util_realloc(void *ptr, size_t size);

/*!
 * \brief  Deallocated provided memory pointer
 *
 * \param  ptr  The pointer
 */
void nsk_util_free(void *ptr);

#endif
