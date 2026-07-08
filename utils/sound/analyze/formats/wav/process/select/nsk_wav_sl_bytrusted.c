#include "nsk_wav_sl_bytrusted.h"
#include "../../../../arguments/nsk_args_options.h"

bool nsk_wav_sl_bytrusted(
    const struct nsk_wav_mark *mark
) {
    if (
        nsk_options_program.profile.select.periodcandidatesthreshold == 0
    ) {
        return false;
    }

    size_t periods =
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE] +
        mark->types[NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE] +
        mark->types[NSK_WAV_CND_METHOD_EDGE_CHANGE];

    return
        periods >= nsk_options_program.profile.select.periodcandidatesthreshold &&
        mark->maxabsdeltast >= nsk_options_program.profile.select.deltastthreshold;
}
