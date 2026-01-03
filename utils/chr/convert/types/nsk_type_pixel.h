#ifndef NSK_TYPE_PIXEL
#define NSK_TYPE_PIXEL

#include <stdint.h>

/*!
 * \brief  Single pixel data
 */
union nsk_type_pixel {
    uint32_t raw;
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
};

#endif