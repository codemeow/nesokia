#ifndef NSK_PPUCOLORS_COMMON
#define NSK_PPUCOLORS_COMMON

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

#endif