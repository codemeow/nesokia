#ifndef NSK_PNG_READ
#define NSK_PNG_READ

#include "../types/nsk_type_image.h"

/*!
 * \brief  Reads the selected PNG file into the localimage format
 *
 * \param[in] filename  The filename
 * \return Allocated image
 */
struct nsk_image *nsk_png_read(const char *filename);

#endif