#ifndef NSK_OUTPUT_NAMETABLE
#define NSK_OUTPUT_NAMETABLE

#include <stddef.h>

/*!
 * \brief  Converts the provided nametable to the output file
 *
 * \param[in] x     Local palette X
 * \param[in] y     Local palette Y
 * \param[in] dir   Directory to save or NULL for current
 * \param[in] name  The name to save
 */
void nsk_output_nametable(
    size_t x,
    size_t y,
    const char *dir,
    const char *name
);

#endif