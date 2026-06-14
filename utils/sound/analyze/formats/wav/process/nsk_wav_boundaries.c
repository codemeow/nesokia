#include <nsk_util_meta.h>

#include "nsk_wav_boundaries.h"

#include "boundaries/nsk_wav_bf_rmsenv.h"

/*!
 * \brief  Finds the boundaries between notes
 *
 * <TODO>
 *
 * \param[in,out]  wav   The wav
 * \param[in]      cnds  The list of candidates
 */
bool nsk_wav_boundaries(
    struct nsk_wav      *wav,
    struct nsk_wav_cnds *cnds
) {
    // TODO declare function that runs over arrays

    static const struct {
        const char *name;
        bool (*func)(
            const struct nsk_wav  *wav,
            struct nsk_wav_cnds   *cnds
        );
    } _table[] = {
        {
            .name = "Active spans by RMS envelope",
            .func = nsk_wav_bf_rmsenv
        }
    };

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        nsk_inf("    Step: %s\n", _table[i].name);
        if (!_table[i].func(wav, cnds)) {
            return false;
        }
    }

    return true;
}