#include <inttypes.h>
#include <stdio.h>

#include "nsk_util_hrbytes.h"

#include "nsk_util_size.h"

/*!
 * \brief  Converts bytes into human-readable format (KB/MB/GB + raw value)
 *
 * \param[in] value  The value
 * \return Static string
 */
const char *nsk_util_hrbytes(uint64_t value) {
    static char result[128];
    static const struct {
        const char *unit;
        uint64_t multiplier;
    } units[] = {
        { "KiB", UINT64_C(1) << 10},
        { "MiB", UINT64_C(1) << 20},
        { "GiB", UINT64_C(1) << 30},
        { "TiB", UINT64_C(1) << 40},
    };
    static const size_t maxunit = NSK_SIZE(units);

    if (value < units[0].multiplier) {
        snprintf(
            result,
            sizeof(result),
            "%" PRIu64 " %s",
            value,
            value == 1 ? "byte" : "bytes"
        );
        return result;
    }

    size_t i = 0;
    double scaled;

    do {
        scaled = (double)value / units[i].multiplier;
        i++;
    } while (i < maxunit && scaled >= 512.0);

    snprintf(
        result,
        sizeof(result),
        "%.2f %s (%" PRIu64 " bytes)",
        scaled,
        units[i - 1].unit,
        value
    );
    return result;
}
