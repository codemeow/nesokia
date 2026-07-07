#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "nsk_math_fft.h"
#include "../log/nsk_log_err.h"
#include "../base/nsk_util_cleanup.h"

/*!
 * \brief  Checks whether a value is a power of two
 *
 * \param[in] value  Value to test
 * \return    True if value is a non-zero power of two
 */
static bool _ispowerof2(
    size_t value
) {
    return value != 0 && (value & (value - 1)) == 0;
}

/*!
 * \brief  Returns the bit-reversed index for an FFT buffer
 *
 * \param[in] index  Source index
 * \param[in] count  FFT size
 * \return    Bit-reversed index
 */
static size_t _bitreverse(
    size_t index,
    size_t count
) {
    size_t reversed = 0;

    for (size_t bits = count; bits > 1; bits >>= 1) {
        reversed = (reversed << 1) | (index & 1);
        index >>= 1;
    }

    return reversed;
}

/*!
 * \brief  Calculates a forward complex FFT
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
) {
    if (
        !inreal  ||
        !outreal ||
        !outimag ||
        !_ispowerof2(count)
    ) {
        nsk_err("Cannot calculate FFT for invalid input or output arrays");
        return false;
    }

    nsk_auto_free double *real = calloc(count, sizeof(*real));
    nsk_auto_free double *imag = calloc(count, sizeof(*imag));

    if (!real || !imag) {
        nsk_err("Cannot allocate memory for FFT calculation");
        return false;
    }

    for (size_t i = 0; i < count; i++) {
        const size_t j = _bitreverse(i, count);
        real[j] = inreal[i];
        imag[j] = inimag ? inimag[i] : 0.0;
    }

    for (size_t length = 2; length <= count; length <<= 1) {
        const double angle = -2.0 * M_PI / length;
        const double stepreal = cos(angle);
        const double stepimag = sin(angle);

        for (size_t offset = 0; offset < count; offset += length) {
            double factorreal = 1.0;
            double factorimag = 0.0;

            for (size_t i = 0; i < length / 2; i++) {
                const size_t left = offset + i;
                const size_t right = left + length / 2;

                const double evenreal = real[left];
                const double evenimag = imag[left];
                const double oddreal =
                    real[right] * factorreal -
                    imag[right] * factorimag;
                const double oddimag =
                    real[right] * factorimag +
                    imag[right] * factorreal;

                real[left] = evenreal + oddreal;
                imag[left] = evenimag + oddimag;
                real[right] = evenreal - oddreal;
                imag[right] = evenimag - oddimag;

                const double nextreal =
                    factorreal * stepreal -
                    factorimag * stepimag;
                const double nextimag =
                    factorreal * stepimag +
                    factorimag * stepreal;

                factorreal = nextreal;
                factorimag = nextimag;
            }
        }
    }

    memcpy(outreal, real, count * sizeof(*outreal));
    memcpy(outimag, imag, count * sizeof(*outimag));

    return true;
}

/*!
 * \brief  Calculates a forward FFT for real input
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
) {
    if (
        !input                  ||
        !outreal                ||
        !outimag                ||
        inputcount > fftsize    ||
        !_ispowerof2(fftsize)
    ) {
        nsk_err("Cannot calculate real FFT for invalid input or output arrays");
        return false;
    }

    nsk_auto_free double *padded   = calloc(fftsize, sizeof(*padded));
    nsk_auto_free double *fullreal = calloc(fftsize, sizeof(*fullreal));
    nsk_auto_free double *fullimag = calloc(fftsize, sizeof(*fullimag));

    if (!padded || !fullreal || !fullimag) {
        nsk_err("Cannot allocate memory for real FFT calculation");
        return false;
    }

    memcpy(padded, input, inputcount * sizeof(*input));

    if (
        !nsk_math_fftd(
            padded,
            NULL,
            fftsize,
            fullreal,
            fullimag
        )
    ) {
        return false;
    }

    const size_t bincount = fftsize / 2 + 1;

    memcpy(outreal, fullreal, bincount * sizeof(*outreal));
    memcpy(outimag, fullimag, bincount * sizeof(*outimag));

    return true;
}
