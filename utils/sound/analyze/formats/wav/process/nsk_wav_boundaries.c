#include <nsk_util_meta.h>

#include "nsk_wav_boundaries.h"
#include "../nsk_wav_utils.h"

#include "boundaries/nsk_wav_bf_edgetrains.h"
#include "boundaries/nsk_wav_bf_grid.h"
#include "boundaries/nsk_wav_bf_rmsenv.h"
#include "boundaries/nsk_wav_bf_schmitt.h"

/*!
 * \brief  Finds the boundaries between notes
 *
 * Runs all configured boundary detectors and accumulates candidates in the
 * processing context.
 *
 * \param[in]      wav  Source WAV data
 * \param[in,out]  ctx  Processing context
 * \return True if all boundary detectors completed successfully
 */
bool nsk_wav_boundaries(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    // TODO declare function that runs over arrays

    static const struct {
        const char *name;
        bool (*func)(
            const struct nsk_wav *wav,
            struct nsk_wav_ctx  *ctx
        );
    } _table[] = {
        {
            .name = "Active spans by RMS envelope",
            .func = nsk_wav_bf_rmsenv
        },
        {
            .name = "Schmitt-trigger edges",
            .func = nsk_wav_bf_schmitt
        },
        {
            .name = "Same polarity edge trains",
            .func = nsk_wav_bf_edgetrains
        },
        {
            .name = "Energy rise on 60/50 Hz grid",
            .func = nsk_wav_bf_grid
        }
    };

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        nsk_inf("    ## %s\n", _table[i].name);
        const double start = nsk_time_stamp();
        if (!_table[i].func(wav, ctx)) {
            return false;
        }
        const double end = nsk_time_stamp();
        nsk_time_log(start, end, 8);
    }

    return true;
}
