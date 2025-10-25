#ifndef NSK_IO_FOPEN
#define NSK_IO_FOPEN

#include <stdio.h>

/*!
 * \brief  Platform-specific version of fopen
 *
 * \param[in] filename  The filename
 * \param[in] mode      The mode
 * \return Opened file or NULL
 */
FILE *nsk_io_fopen(const char *filename, const char *mode);

#endif