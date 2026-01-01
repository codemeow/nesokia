#include <threads.h>
#include <stdio.h>

#include "../strings/nsk_strings_ansi.h"

/*!
 * \brief  Number of static strings available in one function call
 */
#define _STATIC_CAROUSEL_SIZE (5)

/*!
 * \brief Opens the ANSI sequence
 */
#define NSK_ANSI_OPEN "\033["

/*!
 * \brief Closes the ANSI sequence
 */
#define NSK_ANSI_CLOSE "m"

/*!
 * \brief ANSI background color control code
 */
#define NSK_ANSI_BACKGROUND_COLOR "48"

/*!
 * \brief ANSI foreground color control code
 */
#define NSK_ANSI_FOREGROUND_COLOR "38"

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
const char *nsk_ansi_24bit(uint8_t r, uint8_t g, uint8_t b, bool back) {
    static thread_local char string[_STATIC_CAROUSEL_SIZE][128];
    static thread_local size_t index;

    if (++index >= _STATIC_CAROUSEL_SIZE) {
        index = 0;
    }

    snprintf(
        string[index],
        sizeof(string[index]),
        NSK_ANSI_OPEN "%s;2;%u;%u;%u" NSK_ANSI_CLOSE,
        back ? NSK_ANSI_BACKGROUND_COLOR : NSK_ANSI_FOREGROUND_COLOR,
        r,
        g,
        b
    );

    return string[index];
}

/*!
 * \brief  Returns the ANSI-escape sequence, containing style reset code
 *
 * \return Static string
 */
const char *nsk_ansi_reset(void) {
    static const char string[] = NSK_ANSI_OPEN "0" NSK_ANSI_CLOSE;
    return string;
}