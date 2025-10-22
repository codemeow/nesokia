#include <string.h>

#include "../utils/nsk_util_malloc.h"
#include "../log/nsk_log_err.h"

/*!
 * \brief  Allocates the required amount of memory and fill it with zeroes
 *
 * \param[in] size  The size
 * \return Allocated memory
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
 * \brief  Deallocated provided memory pointer
 *
 * \param  ptr  The pointer
 */
void nsk_util_free(void *ptr) {
    free(ptr);
}

