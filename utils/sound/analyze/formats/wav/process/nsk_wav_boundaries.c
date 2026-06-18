#include <nsk_util_meta.h>

#include "nsk_wav_boundaries.h"

#include "boundaries/nsk_wav_bf_rmsenv.h"

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
        }
    };

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        size_t old = wav->candidates.count;
        nsk_inf("    Step: %s\n", _table[i].name);
        if (!_table[i].func(wav)) {
            return false;
        }
        size_t new = wav->candidates.count;
        nsk_inf("        (%+zd candidates)\n", (ssize_t)new - old);
    }

    return true;
}