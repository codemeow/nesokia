#ifndef NSK_UTILS_MALLOC
#define NSK_UTILS_MALLOC

#include <stdlib.h>

/*!
 * \brief  Allocates the required amount of memory and fill it with zeroes
 *
 * \param[in] size  The size
 * \return Allocated memory
 */
void *nsk_util_malloc(size_t size);

/*!
 * \brief  Deallocated provided memory pointer
 *
 * \param  ptr  The pointer
 */
void nsk_util_free(void *ptr);

#endif