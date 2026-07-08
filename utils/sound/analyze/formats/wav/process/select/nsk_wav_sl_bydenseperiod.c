#include "nsk_wav_sl_bydenseperiod.h"
#include "nsk_wav_sl_utils.h"
#include "../../../../arguments/nsk_args_options.h"

bool nsk_wav_sl_bydenseperiod(
    const struct nsk_wav_mark *mark
) {
    if (!mark->indense) {
        return false;
    }

    const size_t periods =
        mark->types[NSK_WAV_CND_METHOD_MIXED_PERIOD_SOLVE] +
        mark->types[NSK_WAV_CND_METHOD_SHORT_PERIOD_EDGE] +
        mark->types[NSK_WAV_CND_METHOD_EDGE_CHANGE];

    return
        periods >=
            nsk_options_program.profile.select.denseperiodsthreshold &&
        mark->maxabsdeltast >=
            nsk_options_program.profile.select.densedeltastthreshold &&
        mark->maxstrength >=
            nsk_options_program.profile.select.densestrengththreshold &&
        nsk_wav_sl_griderror(
            mark,
            nsk_wav_sl_filter_period
        ) <= nsk_options_program.profile.quantization.windowerror;
}
