#include <stdio.h>

#include "strings/nsk_strings_ansi.h"
#include "base/nsk_util_cleanup.h"
#include "base/nsk_util_malloc.h"

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
 * \brief Declares the support of ANSI codes
 */
bool nsk_ansi_support = true;

/*!
 * \brief  Returns an allocated string, containing ANSI-sequence to set the
 * current background or foreground color as 24-bit color
 *
 * \param[in] r     Red component
 * \param[in] g     Green component
 * \param[in] b     Blue component
 * \param[in] back  True if background code required, false otherwise
 * \return Allocated string
 */
char *nsk_ansi_24bit(uint8_t r, uint8_t g, uint8_t b, bool back) {
    char *string = nsk_util_malloc(128);

    snprintf(
        string,
        128,
        NSK_ANSI_OPEN "%s;2;%u;%u;%u" NSK_ANSI_CLOSE,
        back ? NSK_ANSI_BACKGROUND_COLOR : NSK_ANSI_FOREGROUND_COLOR,
        r,
        g,
        b
    );

    return string;
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

static uint8_t _color_lumafg(uint8_t r, uint8_t g, uint8_t b) {
    /* ITU-R BT.709 luma (sRGB-ish) */
    uint8_t y = (0.2126 * r + 0.7152 * g + 0.0722 * b);
    return (y < 150) ? 255 : 0;
}

/*!
 * \brief  Returns allocated string, containing (or not) the ANSI sequence
 * and the color code itself along with the closing sequence
 *
 * \param[in] r   Red component
 * \param[in] g   Green component
 * \param[in] b   Blue component
 *
 * \return Allocated string
 */
char *nsk_string_color(uint8_t r, uint8_t g, uint8_t b) {
    char *string = nsk_util_malloc(128);

    if (nsk_ansi_support) {
        uint8_t fg = _color_lumafg(r, g, b);
        nsk_auto_free char *bg = nsk_ansi_24bit(r, g, b, true);
        nsk_auto_free char *text = nsk_ansi_24bit(fg, fg, fg, false);

        snprintf(
            string,
            128,
            "%s" "%s" "%02x%02x%02x" "%s",
            bg,
            /*
             * Most of the colors will make the
             * default text color unreadable.
             * We will invert the foreground
             * color to mostly solve it
             */
            text,
            r,
            g,
            b,
            nsk_ansi_reset()
        );

    } else {
        snprintf(
            string,
            128,
            "%02x%02x%02x",
            r,
            g,
            b
        );
    }

    return string;
}
