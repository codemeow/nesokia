#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "types/ppucolors/nsk_ppucolors_common.h"
#include "log/nsk_log_err.h"
#include "error/nsk_util_errno.h"
#include "io/nsk_io_fopen.h"
#include "base/nsk_util_malloc.h"

/*!
 * \brief  Converts string value to UTF16-BE format
 *
 * \warning Only supports ASCII-characters (codes < 128)
 *
 * \param[in]  str   The ASCII string
 * \param[out] size  The result size in 16-bit code units
 * \return Allocated UTF16-BE buffer, or NULL if conversion fails
 *
 * \note Arguments cannot be NULL.
 */
uint8_t *nsk_ppucolors_toUTF16be(
    const char *str,
    size_t *size
) {
    size_t namelen = strlen(str);

    *size = (namelen + 1);
    for (size_t i = 0; i < namelen; i++) {
        uint8_t c = str[i];
        if (c >= 0x80) {
            return NULL;
        }
    }

    uint8_t *string = nsk_util_malloc(*size * sizeof(uint16_t));

    for (size_t i = 0; i < namelen; i++) {
        string[i * 2 + 0] = 0;
        string[i * 2 + 1] = str[i];
    }

    string[2 * namelen + 0] = 0;
    string[2 * namelen + 1] = 0;

    return string;
}

/*!
 * \brief  Opens the file and handles errors
 *
 * \param[in] filename  The filename
 * \param[in] mode      The mode
 * \return  File descriptor, or NULL on open error
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
        return NULL;
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
 * \return True if the data was written, false otherwise
 */
bool nsk_ppucolors_fwrite(
    const void *buffer,
    size_t size,
    FILE *file,
    const char *filename
) {
    if (fwrite(buffer, size, 1, file) != 1) {
        nsk_err(
            "Error: cannot write %zu bytes to file \"%s\" for PPU colors writing.\n"
            "Possible reason: %s\n",
            size,
            filename,
            nsk_util_strerror(errno)
        );
        return false;
    }

    return true;
}

/*!
 * \brief  Reads the data from the file and handles the errors
 *
 * \param[out]    buffer   The buffer
 * \param[in]     size     The size
 * \param[in,out] file     The file
 * \param[in]     filename The file's filename
 * \return True if the data was read, false otherwise
 */
bool nsk_ppucolors_fread(
    void *buffer,
    size_t size,
    FILE *file,
    const char *filename
) {
    if (fread(buffer, size, 1, file) != 1) {
        nsk_err(
            "Error: cannot read %zu bytes file \"%s\" for PPU colors reading.\n"
            "Possible reason: %s\n",
            size,
            filename,
            nsk_util_strerror(errno)
        );
        return false;
    }

    return true;
}
