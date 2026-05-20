#include <string.h>

#include "base/nsk_util_malloc.h"

#include "log/nsk_log_err.h"

/*!
 * \brief  Allocates the required amount of memory and fills it with zeroes
 *
 * \param[in] size  The size
 * \return Allocated memory
 *
 * \note This function never returns NULL. It either returns allocated memory
 *       or terminates the process.
 */
void *nsk_util_malloc(size_t size) {
    void *ptr = calloc(size, 1);
    if (!ptr) {
        nsk_err(
            "Error: cannot allocate %zu bytes\n",
            size
        );
        abort();
    }
    return ptr;
}

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
void *nsk_util_realloc(void *ptr, size_t size) {
    void *old = ptr;
    ptr = realloc(ptr, size);
    if (!ptr) {
        nsk_util_free(old);
        nsk_err(
            "Error: cannot reallocate %zu bytes\n",
            size
        );
        abort();
    }
    return ptr;
}

/*!
 * \brief  Deallocated provided memory pointer
 *
 * \param  ptr  The pointer
 */
void nsk_util_free(void *ptr) {
    free(ptr);
}
