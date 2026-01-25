#include <errno.h>
#include <stdlib.h>

#include "../../types/ppucolors/nsk_ppucolors_common.h"
#include "../../log/nsk_log_err.h"
#include "../../error/nsk_util_errno.h"
#include "../../io/nsk_io_fopen.h"

/*!
 * \brief  Opens the file and handles errors
 *
 * \param[in] filename  The filename
 * \param[in] mode      The mode
 * \return  file descriptor
 */
FILE *nsk_ppucolors_fopen(
    const char *filename,
    const char *mode
) {
    FILE *file = nsk_io_fopen(
        filename,
        mode
    );
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\" for PPU colors writing.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }
    return file;
}

/*!
 * \brief  Writes the data to the file and handles the errors
 *
 * \param[in]     buffer   The buffer
 * \param[in]     size     The size
 * \param[in,out] file     The file
 * \param[in]     filename The file's filename
 */
void nsk_ppucolors_fwrite(
    const void *buffer,
    size_t size,
    FILE *file,
    const char *filename
) {
    if (fwrite(buffer, size, 1, file) != 1) {
        nsk_err(
            "Error: cannot write file \"%s\" for PPU colors writing.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }
}

/*!
 * \brief  Reads the data from the file and handles the errors
 *
 * \param[out]    buffer   The buffer
 * \param[in]     size     The size
 * \param[in,out] file     The file
 * \param[in]     filename The file's filename
 */
void nsk_ppucolors_fread(
    void *buffer,
    size_t size,
    FILE *file,
    const char *filename
) {
    if (fread(buffer, size, 1, file) != 1) {
        nsk_err(
            "Error: cannot read file \"%s\" for PPU colors reading.\n"
            "Possible reason: %s\n",
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }
}