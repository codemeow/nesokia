#include <math.h>

#include "../utils/nsk_util_expsize.h"

/*!
 * \brief  Converts hi and low parts (MSB/LSB) to size in bytes:
 * - `(hi << 8 | low) * multiplier`             (if MSB != 0xf)
 * - `(2^low:11111100 * (low:00000011 * 2 + 1)` (if MSB == 0xf)
 *
 * \note Passing hi as 0 allows to used old conversion format (iNES)
 *
 * \param[in] hi            MSB byte (only `0000 1111` bits)
 * \param[in] low           LSB byte
 * \param[in] multiplier    The multiplier
 * \return Calculated size in bytes
 */
uint64_t nsk_util_expsize(uint8_t hi, uint8_t low, uint32_t multiplier) {
    union {
        uint8_t byte;
        struct {
            uint8_t m:2;
            uint8_t e:6;
        } bits;
    } conv = {
        .byte = low
    };

    if (hi == 0xf) {
        return pow(2, conv.bits.e) * (conv.bits.m * 2 + 1);

    } else {
        return (hi << 8 | low) * multiplier;
    }
}