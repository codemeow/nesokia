#include <stdbool.h>
#include <stdio.h>
#include <threads.h>

#include "../process/nsk_process_utils.h"
#include "../arguments/nsk_args_options.h"
#include "../utils/strings/nsk_strings_ansi.h"

/*!
 * \brief  Number of static strings available in one function call
 */
#define _STATIC_CAROUSEL_SIZE (5)

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
const char *nsk_string_color(uint8_t r, uint8_t g, uint8_t b) {
    static thread_local char string[_STATIC_CAROUSEL_SIZE][128];
    static thread_local size_t index;

    index++;
    if (index >= _STATIC_CAROUSEL_SIZE) {
        index = 0;
    }

    if (nsk_options_program.output.colored) {
        snprintf(
            string[index],
            sizeof(string[index]),
                "%s"
                "%s"
                "%02x%02x%02x"
                "%s",
            nsk_ansi_24bit(
                r,
                g,
                b,
                true
            ),
            nsk_ansi_24bit(
                255 - r, /* Most of the colors will make the */
                255 - g, /* default text color unreadable.   */
                255 - b, /* We will invert the background    */
                false    /* color to mostly solve it         */
            ),
            r,
            g,
            b,
            nsk_ansi_reset()
        );

    } else {
        snprintf(
            string[index],
            sizeof(string[index]),
            "%02x%02x%02x",
            r,
            g,
            b
        );
    }

    return string[index];
}