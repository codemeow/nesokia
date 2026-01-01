#ifndef NSK_STRINGS_ANSI
#define NSK_STRINGS_ANSI

#include <stdbool.h>
#include <stdint.h>

/*!
 * \brief  Returns the static string, containing ANSI-sequence to set the
 * current background or foreground color as 24-bit color
 *
 * \param[in] r     Red component
 * \param[in] g     Green component
 * \param[in] b     Blue component
 * \param[in] back  True if background code required, false otherwise
 * \return Static string
 */
const char *nsk_ansi_24bit(uint8_t r, uint8_t g, uint8_t b, bool back);

/*!
 * \brief  Returns the ANSI-escape sequence, containing style reset code
 *
 * \return Static string
 */
const char *nsk_ansi_reset(void);

#endif