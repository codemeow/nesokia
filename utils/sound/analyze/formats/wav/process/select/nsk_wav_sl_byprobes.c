#include "nsk_wav_sl_byprobes.h"
#include "nsk_wav_sl_utils.h"
#include "../../../../arguments/nsk_args_options.h"

bool nsk_wav_sl_byprobes(
    const struct nsk_wav_mark *mark
) {
    if (!mark->indense) {
        return false;
    }

    return
        mark->types[NSK_WAV_CND_METHOD_TRANSITION_PROBES] >=
            nsk_options_program.profile.select.densetransitionsthreshold &&
        mark->types[NSK_WAV_CND_METHOD_EDGE_PROBE] == 0 &&
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE] == 0 &&
        mark->types[NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE] == 0 &&
        mark->types[NSK_WAV_CND_METHOD_EDGE_CHANGE] == 0 &&
        mark->maxstrength >=
            nsk_options_program.profile.select.transitionstrengththreshold &&
        mark->maxabsdeltast >=
            nsk_options_program.profile.select.transitiondeltastthreshold &&
        nsk_wav_sl_griderror(
            mark,
            nsk_wav_sl_filter_transition
        ) <= nsk_options_program.profile.select.energygriderror;
}
