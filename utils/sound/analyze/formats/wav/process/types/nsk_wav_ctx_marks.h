#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "nsk_wav_ctx_candidates.h"

/*!
 * \brief  Quantized time mark
 */
struct nsk_wav_mark {
    size_t frame;                                /*!< Frame index */
    size_t count;                                /*!< Number of candidates */
    const struct nsk_wav_candidate **candidates; /*!< Candidates list */

    bool accepted; /*!< Whether this mark is accepted as a note change */

    double maxstrength;   /*!< Maximum strength of all candidates */
    double maxabsdeltast; /*!< Maximum absolute deltast of all candidates */

    size_t types[NSK_WAV_CND_METHODS_COUNT]; /*!< Candidate count by method */

    bool indense; /*!< Whether this mark is inside a dense accepted area */
};

/*!
 * \brief  Quantized mark storage owned by the processing context
 */
struct nsk_wav_ctx_marks {
    size_t count;              /*!< Number of marks */
    struct nsk_wav_mark *list; /*!< Mark list */
};
