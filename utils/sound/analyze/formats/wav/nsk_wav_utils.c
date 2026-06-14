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


/*!
 * \brief  Autocleaner for allocated WAV candidates data
 *
 * \param[in, out]  cnds  The WAV candidates data
 */
void _nsk_auto_cnds(struct nsk_wav_cnds **cnds) {
    if (!cnds || !*cnds) {
        return;
    }

    nsk_wav_cnds_free(*cnds);
}