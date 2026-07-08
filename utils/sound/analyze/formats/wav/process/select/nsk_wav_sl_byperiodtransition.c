#include "nsk_wav_sl_byperiodtransition.h"
#include "../../../../arguments/nsk_args_options.h"

bool nsk_wav_sl_byperiodtransition(
    const struct nsk_wav_mark *mark
) {
    if (
        nsk_options_program.profile.select.transitionperiodcandidatesthreshold == 0 ||
        nsk_options_program.profile.select.transitionprobecandidatesthreshold  == 0
    ) {
        return false;
    }

    const size_t periods =
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE] +
        mark->types[NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE] +
        mark->types[NSK_WAV_CND_METHOD_EDGE_CHANGE];

    return
        periods >=
            nsk_options_program.profile.select.transitionperiodcandidatesthreshold &&
        mark->types[NSK_WAV_CND_METHOD_TRANSITION_PROBES] >=
            nsk_options_program.profile.select.transitionprobecandidatesthreshold &&
        mark->maxstrength >=
            nsk_options_program.profile.select.transitionstrengththreshold &&
        mark->maxabsdeltast >=
            nsk_options_program.profile.select.transitiondeltastthreshold;
}
