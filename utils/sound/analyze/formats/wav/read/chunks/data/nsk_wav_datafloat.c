#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <float.h>

#include <nsk_util_meta.h>

#include "nsk_wav_datafloat.h"
#include "../../nsk_wav_readers.h"

/*!
 * \brief  Reads sample value from the file as 32-bit float value
 *
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the valus is successfully read
 */
static bool _wav_reader_datafloat32(
    FILE   *file,
    double *value
) {
    union {
        uint32_t u;
        float    f;
    } conv;

    if (!nsk_wav_read_u32le(file, &conv.u)) {
        return false;
    }

    *value = conv.f;
    return true;
}

/*!
 * \brief  Reads sample value from the file as 64-bit float value
 *
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the valus is successfully read
 */
static bool _wav_reader_datafloat64(
    FILE   *file,
    double *value
) {
    union {
        uint64_t u;
        double   f;
    } conv;

    if (!nsk_wav_read_u64le(file, &conv.u)) {
        return false;
    }

    *value = conv.f;
    return true;
}

/*!
 * \brief  Reads one WAV chunk of type DATA in FLOAT format
 *
 * \param[in,out] file          The file descriptor
 * \param[in,out] wav           The wav data
 * \param[in]     chunksize     The expected size of the chunk
 * \param[in,out] rest          The rest number of chunks bytes to read
 */
bool nsk_wav_reader_datafloat(
    FILE           *file,
    struct nsk_wav *wav,
    ssize_t         chunksize,
    ssize_t        *rest
) {
    typedef bool (*_reader)(
        FILE    *file,
        double  *value
    );

    static const struct {
        uint16_t bitspersample;
        _reader  reader;
    } _table[] = {
        {
            .bitspersample = 32,
            .reader        = _wav_reader_datafloat32
        },{
            .bitspersample = 64,
            .reader        = _wav_reader_datafloat64
        }
    };

    _reader reader = NULL;

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (_table[i].bitspersample == wav->format.bitspersample) {
            reader = _table[i].reader;
            break;
        }
    }

    if (!reader) {
        nsk_err(
            "Unsupported WAV bits per sample value for FLOAT: %" PRIu16 "\n",
            wav->format.bitspersample
        );
        return false;
    }

    const size_t bytespersample = wav->format.bitspersample / CHAR_BIT;

    if (wav->format.blockalign != wav->format.channels * bytespersample) {
        nsk_err("Invalid WAV block align\n");
        return false;
    }

    if (chunksize % wav->format.blockalign != 0) {
        nsk_err("Malformed WAV `data` block\n");
        return false;
    }

    wav->samples.raw.count = chunksize / wav->format.blockalign;
    wav->samples.raw.value = calloc(
        wav->samples.raw.count,
        sizeof(wav->samples.raw.value[0])
    );
    if (!wav->samples.raw.value) {
        nsk_err("Cannot allocate memory for WAV sample values\n");
        return false;
    }
    wav->samples.raw.max   = DBL_MIN;
    wav->samples.raw.min   = DBL_MAX;


    for (size_t sample = 0 ; sample < wav->samples.raw.count; sample++) {
        for (size_t channel = 0; channel < wav->format.channels; channel++) {
            double value = 0.0;
            if (!reader(file, &value)) {
                nsk_err("Cannot read sample value from WAV\n");
                return false;
            }
            wav->samples.raw.value[sample] += value;

            chunksize -= bytespersample;
            if (chunksize < 0) {
                nsk_err("Malformed WAV `data` chunk\n");
                return false;
            }

            *rest -= bytespersample;
            if (*rest < 0) {
                nsk_err("Corrupted WAV file\n");
                return false;
            }
        }
        wav->samples.raw.value[sample] /= wav->format.channels;
        if (wav->samples.raw.value[sample] < wav->samples.raw.min) {
            wav->samples.raw.min = wav->samples.raw.value[sample];
        }
        if (wav->samples.raw.value[sample] > wav->samples.raw.max) {
            wav->samples.raw.max = wav->samples.raw.value[sample];
        }
    }

    return true;
}