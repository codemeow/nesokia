#include <nsk_util_meta.h>

#include "nsk_wav_utils.h"
#include "nsk_wav_type.h"

/*!
 * \brief  Autocleaner for allocated WAV data
 *
 * \param[in, out]  wav  The WAV data
 */
void _nsk_auto_wav(struct nsk_wav **wav) {
    if (!wav || !*wav) {
        return;
    }

    nsk_wav_free(*wav);
}

void nsk_time_log(
    double start,
    double end,
    size_t padding
) {
    nsk_inf(
        "%*s- (%.3f ms)\n",
        (int)padding,
        "",
        end - start
    );
}
