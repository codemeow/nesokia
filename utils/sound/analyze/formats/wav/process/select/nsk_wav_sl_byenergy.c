#include <math.h>

#include "nsk_wav_sl_byenergy.h"
#include "../../../../arguments/nsk_args_options.h"

bool nsk_wav_sl_byenergy(
    const struct nsk_wav_mark *mark
) {
    if (
        nsk_options_program.profile.select.energycandidatesthreshold == 0
    ) {
        return false;
    }

    const double marktime =
        (double)mark->frame /
        nsk_options_program.profile.boundary.grid.fps;
    size_t energycandidates = 0;

    for (size_t i = 0; i < mark->count; i++) {
        const struct nsk_wav_candidate *candidate = mark->candidates[i];

        if (
            candidate->method != NSK_WAV_CND_METHOD_ENERGY_ONSET &&
            candidate->method != NSK_WAV_CND_METHOD_ENERGY_OFFSET &&
            candidate->method != NSK_WAV_CND_METHOD_GRID_ENERGY_RISE
        ) {
            continue;
        }

        if (
            fabs(candidate->timestamp - marktime) <=
            nsk_options_program.profile.select.energygriderror
        ) {
            energycandidates++;
            if (
                energycandidates >=
                nsk_options_program.profile.select.energycandidatesthreshold
            ) {
                return true;
            }
        }
    }

    return false;
}
