#pragma once

#include <stdbool.h>
#include <stddef.h>

/*!
 * \brief  Calculates a forward complex FFT
 *
 * The transform is out-of-place: input arrays are read-only and output arrays
 * receive the result.  The imaginary input may be NULL, in which case all
 * imaginary input samples are treated as zero.  The output arrays must contain
 * at least count elements.  The count must be a power of two.
 *
 * \param[in]  inreal   Real input samples
 * \param[in]  inimag   Imaginary input samples, or NULL
 * \param[in]  count    Number of input and output samples
 * \param[out] outreal  Real output samples
 * \param[out] outimag  Imaginary output samples
 * \return     True if the transform was calculated
 */
bool nsk_math_fftd(
    const double *inreal,
    const double *inimag,
    size_t        count,
    double       *outreal,
    double       *outimag
);

/*!
 * \brief  Calculates a forward FFT for real input
 *
 * The input is copied into a zero-padded buffer of fftsize samples, transformed
 * with nsk_math_fftd(), and returned as the non-negative frequency bins.  The
 * output arrays must contain at least fftsize / 2 + 1 elements.  The fftsize
 * must be a power of two and must be greater than or equal to inputcount.
 *
 * \param[in]  input       Real input samples
 * \param[in]  inputcount  Number of real input samples
 * \param[in]  fftsize     Transform size
 * \param[out] outreal     Real output bins
 * \param[out] outimag     Imaginary output bins
 * \return     True if the transform was calculated
 */
bool nsk_math_rfftd(
    const double *input,
    size_t        inputcount,
    size_t        fftsize,
    double       *outreal,
    double       *outimag
);
