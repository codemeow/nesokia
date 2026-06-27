#include <nsk_util_meta.h>

#include "nsk_wav_boundaries.h"

#include "boundaries/nsk_wav_bf_edgetrains.h"
#include "boundaries/nsk_wav_bf_grid.h"
#include "boundaries/nsk_wav_bf_rmsenv.h"
#include "boundaries/nsk_wav_bf_schmitt.h"

/*!
 * \brief  Finds the boundaries between notes
 *
 * <TODO>
 *
 * \param[in,out]  wav   The wav
 */
bool nsk_wav_boundaries(
    struct nsk_wav      *wav
) {
    // TODO declare function that runs over arrays

    static const struct {
        const char *name;
        bool (*func)(
            struct nsk_wav  *wav
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
        size_t cold  = wav->candidates.count;

        nsk_inf("    ## %s\n", _table[i].name);
        if (!_table[i].func(wav)) {
            return false;
        }
        size_t cnew  = wav->candidates.count;

        nsk_inf("        - (%+zd candidates)\n",    (ssize_t)cnew - cold);
    }

    return true;
}