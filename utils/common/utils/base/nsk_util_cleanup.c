#include "nsk_util_cleanup.h"

#include "nsk_util_malloc.h"

/*!
 * \brief  Autocleaner for memory pointers
 *
 * \param[in, out]  ptr  The pointer
 */
void _nsk_auto_free(void *ptr) {
    if (!ptr) {
        return;
    }

    void **mem = (void **)ptr;

    nsk_util_free(*mem);
}

/*!
 * \brief  Autocleaner for opened files
 *
 * \param[in, out]  file  The file
 */
void _nsk_auto_fclose(FILE **file) {
    if (!file || !*file) {
        return;
    }

    fclose(*file);
}