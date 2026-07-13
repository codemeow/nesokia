#ifndef NSK_TYPE_COLOR4
#define NSK_TYPE_COLOR4

#include <stdint.h>

/*!
 * \brief  Single color data
 */
union nsk_type_color4 {
    uint32_t raw;
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
};

#endif