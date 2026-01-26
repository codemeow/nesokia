#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

#include "../../types/ppucolors/nsk_ppucolors_common.h"
#include "../../log/nsk_log_err.h"
#include "../../error/nsk_util_errno.h"
#include "../../io/nsk_io_fopen.h"

/*!
 * \brief  Number of static strings available in one function call
 */
#define _STATIC_CAROUSEL_SIZE (5)

/*!
 * \brief  Converts name value to UTF16-BE format
 *
 * \warning Only supports ASCII-characters (codes < 128)
 *
 * \param[in]  name  The ASCII name
 * \param[out] size  The result size in bytes
 * \return Static zero-terminated UTF16-BE "string"
 */
const uint8_t *nsk_ppucolors_toUTF16be(
    const char *str,
    size_t *size
) {
    static thread_local uint8_t string[_STATIC_CAROUSEL_SIZE][1024];
    static thread_local size_t index;

    if (++index >= _STATIC_CAROUSEL_SIZE) {
        index = 0;
    }

    size_t namelen = strlen(str);
    if ((namelen * 2) >= sizeof(string[0])) {
        nsk_err(
            "UTF16 str buffer: overflow"
        );
        abort();
    }

    *size = (namelen + 1);
    for (size_t i = 0; i < namelen; i++) {
        uint8_t c = str[i];
        if (c >= 0x80) {
            nsk_err(
                "UTF16 str buffer: unsupported character"
            );
            abort();
        }
        string[index][i * 2 + 0] = 0;
        string[index][i * 2 + 1] = c;
    }

    string[index][2 * namelen + 0] = 0;
    string[index][2 * namelen + 1] = 1;

    return string[index];
}

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
            "Error: cannot write %zu bytes to file \"%s\" for PPU colors writing.\n"
            "Possible reason: %s\n",
            size,
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
            "Error: cannot read %zu bytes file \"%s\" for PPU colors reading.\n"
            "Possible reason: %s\n",
            size,
            filename,
            nsk_util_strerror(errno)
        );
        exit(EXIT_FAILURE);
    }
}