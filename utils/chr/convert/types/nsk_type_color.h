#ifndef NSK_TYPE_COLOR
#define NSK_TYPE_COLOR

#include <stdint.h>

/*!
 * \brief Common constants
 */
enum {
    NSK_COLOR_INVALID = 0xffff /*!< palette.color indicator */
};

/*!
 * \brief  Color data
 */
struct nsk_type_color {
    union {
        uint32_t raw;   /*!< Raw RGB value          */
        struct {
            uint8_t r;  /*!< Red component          */
            uint8_t g;  /*!< Green component        */
            uint8_t b;  /*!< Blue component         */
        };
    } rgb;              /*!< RGB data               */

    struct {
        unsigned color; /*!< Palette color index    */
    } palette;          /*!< Palette data           */
};

#endif