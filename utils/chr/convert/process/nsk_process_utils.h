#ifndef NSK_PROCESS_UTILS
#define NSK_PROCESS_UTILS

#include <stdint.h>

/*!
 * \brief  Returns static string, containing (or not) the ANSI sequence
 * and the color code itself along with the closing sequence
 *
 * \param[in] r   Red component
 * \param[in] g   Green component
 * \param[in] b   Blue component
 *
 * \return Static string
 */
const char *nsk_string_color(uint8_t r, uint8_t g, uint8_t b);

#endif