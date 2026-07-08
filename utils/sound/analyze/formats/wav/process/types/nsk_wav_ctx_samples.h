#pragma once

#include <stddef.h>

/*!
 * \brief  Working sample buffer used by the WAV processing pipeline
 */
struct nsk_wav_ctx_samples {
    size_t  count;  /*!< Number of samples */
    double *value;  /*!< Centered and peak-normalized sample values */
    double  max;    /*!< Maximum source value */
    double  min;    /*!< Minimum source value */
    double  center; /*!< Center value subtracted from the source samples */
};
