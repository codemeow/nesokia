#include <nsk_util_meta.h>

#include "nsk_wav_boundaries.h"

#include "boundaries/nsk_wav_bf_rmsenv.h"
#include "boundaries/nsk_wav_bf_schmitt.h"
#include "boundaries/nsk_wav_bf_edgetrains.h"

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
        }
    };

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        size_t cold  = wav->candidates.count;
        size_t eoldr = wav->edges.rise.count;
        size_t eoldf = wav->edges.fall.count;

        nsk_inf("    Step: %s\n", _table[i].name);
        if (!_table[i].func(wav)) {
            return false;
        }
        size_t cnew  = wav->candidates.count;
        size_t enewr = wav->edges.rise.count;
        size_t enewf = wav->edges.fall.count;

        nsk_inf("        (%+zd candidates)\n",    (ssize_t)cnew - cold);
        nsk_inf("        (%+zd rising edges)\n",  (ssize_t)enewr - eoldr);
        nsk_inf("        (%+zd falling edges)\n", (ssize_t)enewf - eoldf);
    }

    return true;
}