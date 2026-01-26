#ifndef NSK_PPUCOLORS_COMMON
#define NSK_PPUCOLORS_COMMON

#include <stdint.h>
#include <stdio.h>

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
);

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
);

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
);

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
);

#endif