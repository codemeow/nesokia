#pragma once

#include <stdbool.h>

#include "../types/nsk_wav_ctx_marks.h"

/*!
 * \brief  Checks if the candidate method is a transition probe.
 *
 * \param[in] method  Candidate method
 * \return True if the method is a transition probe method
 */
bool nsk_wav_sl_filter_transition(
    enum nsk_wav_cnd_method method
);

/*!
 * \brief  Checks if the candidate method is a mixed-period solver.
 *
 * \param[in] method  Candidate method
 * \return True if the method is a mixed-period solver method
 */
bool nsk_wav_sl_filter_mixed(
    enum nsk_wav_cnd_method method
);

/*!
 * \brief  Checks if the candidate method is period-transition evidence.
 *
 * \param[in] method  Candidate method
 * \return True if the method is period-transition evidence
 */
bool nsk_wav_sl_filter_period(
    enum nsk_wav_cnd_method method
);

/*!
 * \brief  Finds the smallest grid error for candidates matching a filter.
 *
 * \param[in] mark    Quantized mark
 * \param[in] filter  Candidate method predicate
 * \return Minimum absolute distance from the mark frame time
 */
double nsk_wav_sl_griderror(
    const struct nsk_wav_mark *mark,
    bool (*filter)(
        enum nsk_wav_cnd_method method
    )
);
