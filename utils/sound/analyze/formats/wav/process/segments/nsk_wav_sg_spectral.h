#pragma once

#include "nsk_wav_sg_types.h"

/*!
 * \brief  Calculates real FFT magnitudes for a windowed signal.
 *
 * \param[in]  input  Input sample array.
 * \param[in]  inputcount  Number of input samples.
 * \param[in]  fftsize  FFT size.
 * \param[out]  magnitude  Output magnitude array.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_rfftmagnitude(
    const double *input,
    size_t inputcount,
    size_t fftsize,
    double *magnitude
);

/*!
 * \brief  Interpolates spectral amplitude at a fractional FFT bin.
 *
 * \param[out]  magnitude  Output magnitude array.
 * \param[in]  bincount  Number of magnitude bins.
 * \param[in]  bin  Fractional FFT bin.
 * \return Calculated floating-point value.
 */
double nsk_wav_sg_spectralamplitude(
    const double *magnitude,
    size_t        bincount,
    double        bin
);

/*!
 * \brief  Rounds an integer up to the next power of two.
 *
 * \param[in]  x  Integer value to round up.
 * \return Calculated size value.
 */
size_t nsk_wav_sg_ceil_pow2(size_t x);

/*!
 * \brief  Classifies pitch and duty for a sample window.
 *
 * \param[in]  wav  Source WAV data.
 * \param[in,out]  ctx  Processing context used by the segment pipeline.
 * \param[in]  coresamplestart  Inclusive core sample start.
 * \param[in]  coresampleend  Exclusive core sample end.
 * \param[in,out]  framecount  Number of frames in the analyzed window or repair group.
 * \param[out]  pitch  Calculated pitch result.
 * \return True on success, false on failure.
 */
bool nsk_wav_sg_analyze_window(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    size_t coresamplestart,
    size_t coresampleend,
    size_t framecount,
    struct _segment_pitch *pitch
);
