#if defined(NSK_OS_GNULINUX)

#include <stdio.h>

#include "../../io/nsk_io_fopen.h"

/*!
 * \brief  Platform-specific version of fopen
 *
 * \param[in] filename  The filename
 * \param[in] mode      The mode
 * \return Opened file or NULL
 */
FILE *nsk_io_fopen(const char *filename, const char *mode) {
    return fopen(filename, mode);
}

#endif