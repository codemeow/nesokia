#include <math.h>

#include <nsk_util_meta.h>

#include "nsk_wav_sl_bysinglemixedperiod.h"
#include "nsk_wav_sl_utils.h"
#include "../../../../arguments/nsk_args_options.h"

bool nsk_wav_sl_bysinglemixedperiod(
    const struct nsk_wav_mark *mark
) {
    if (
        nsk_options_program.profile.select.singlemixedcandidatesthreshold == 0
    ) {
        return false;
    }

    double strengthmax = 0.0;
    double deltastmax = 0.0;
    const double griderrormin = nsk_wav_sl_griderror(
        mark,
        nsk_wav_sl_filter_mixed
    );

    for (size_t i = 0; i < mark->count; i++) {
        const struct nsk_wav_candidate *candidate = mark->candidates[i];

        if (candidate->method != NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE) {
            continue;
        }

        strengthmax = NSK_MAX(strengthmax, candidate->strength);
        deltastmax = NSK_MAX(deltastmax, fabs(candidate->deltast));
    }

    return
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE] >=
            nsk_options_program.profile.select.singlemixedcandidatesthreshold &&
        strengthmax >=
            nsk_options_program.profile.select.singlemixedstrengththreshold &&
        deltastmax >=
            nsk_options_program.profile.select.singlemixeddeltastthreshold &&
        griderrormin <=
            nsk_options_program.profile.select.singlemixedgriderror;
}
