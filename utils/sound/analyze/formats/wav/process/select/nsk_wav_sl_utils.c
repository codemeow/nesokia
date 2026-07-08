#include <float.h>
#include <math.h>

#include <nsk_util_meta.h>

#include "nsk_wav_sl_utils.h"
#include "../../../../arguments/nsk_args_options.h"

bool nsk_wav_sl_filter_transition(
    enum nsk_wav_cnd_method method
) {
    return
        method == NSK_WAV_CND_METHOD_TRANSITION_PROBES;
}

bool nsk_wav_sl_filter_mixed(
    enum nsk_wav_cnd_method method
) {
    return
        method == NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE;
}

bool nsk_wav_sl_filter_period(
    enum nsk_wav_cnd_method method
) {
    return
        method == NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE ||
        method == NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE  ||
        method == NSK_WAV_CND_METHOD_EDGE_CHANGE;
}

double nsk_wav_sl_griderror(
    const struct nsk_wav_mark *mark,
    bool (*filter)(
        enum nsk_wav_cnd_method method
    )
) {
    double griderrormin = DBL_MAX;

    const double marktime =
        (double)mark->frame /
        nsk_options_program.profile.boundary.grid.fps;

    for (size_t i = 0; i < mark->count; i++) {
        const struct nsk_wav_candidate *candidate = mark->candidates[i];

        if (!filter(candidate->method)) {
            continue;
        }

        const double griderror = fabs(candidate->timestamp - marktime);
        griderrormin = NSK_MIN(griderrormin, griderror);
    }

    return griderrormin;
}
