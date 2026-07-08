#include "nsk_wav_sl_bystrength.h"
#include "../../../../arguments/nsk_args_options.h"

bool nsk_wav_sl_bystrength(
    const struct nsk_wav_mark *mark
) {
    if (
        nsk_options_program.profile.select.strongcandidatesthreshold == 0
    ) {
        return false;
    }

    size_t strongs =
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE] +
        mark->types[NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE];

    return
        strongs >= nsk_options_program.profile.select.strongcandidatesthreshold &&
        mark->maxstrength >= nsk_options_program.profile.select.strengththreshold;
}
