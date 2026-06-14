#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

#include <nsk_util_meta.h>

#include "nsk_wav_datapcm.h"
#include "../../nsk_wav_readers.h"

/*!
 * \brief  Reads sample value from the file as 8-bit unsigned value
 *
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the valus is successfully read
 */
static bool _wav_reader_datapcm8(
    FILE   *file,
    double *value
) {
    uint8_t raw = 0;
    if (!nsk_wav_read_u8(file, &raw)) {
        nsk_err("Cannot read 8 bit sample value in the WAV file\n");
        return false;
    }

    *value = ((double)raw - 128.0) / 128.0;
    return true;
}

/*!
 * \brief  Reads sample value from the file as 16-bit signed value
*
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the valus is successfully read
 */
static bool _wav_reader_datapcm16(
    FILE   *file,
    double *value
) {
    int16_t raw = 0;
    if (!nsk_wav_read_i16le(file, &raw)) {
        nsk_err("Cannot read 16 bit sample value in the WAV file\n");
        return false;
    }

    *value = (double)raw / 32768.0;
    return true;
}

/*!
 * \brief  Reads sample value from the file as 24-bit signed value
*
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the valus is successfully read
 */
static bool _wav_reader_datapcm24(
    FILE   *file,
    double *value
) {
    int32_t raw = 0;
    if (!nsk_wav_read_i24le(file, &raw)) {
        nsk_err("Cannot read 24 bit sample value in the WAV file\n");
        return false;
    }

    *value = (double)raw / 8388608.0;
    return true;
}

/*!
 * \brief  Reads sample value from the file as 32-bit signed value
*
 * \param[in,out]  file   The file descriptor
 * \param[out]     value  The value
 * \return True if the valus is successfully read
 */
static bool _wav_reader_datapcm32(
    FILE   *file,
    double *value
) {
    int32_t raw = 0;
    if (!nsk_wav_read_i32le(file, &raw)) {
        nsk_err("Cannot read 32 bit sample value in the WAV file\n");
        return false;
    }

    *value = (double)raw / 2147483648.0;
    return true;
}

/*!
 * \brief  Reads one WAV chunk of type DATA in PCM format
 *
 * \param[in,out] file          The file descriptor
 * \param[in,out] wav           The wav data
 * \param[in]     chunksize     The expected size of the chunk
 * \param[in,out] rest          The rest number of chunks bytes to read
 */
bool nsk_wav_reader_datapcm(
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
            .bitspersample = 8,
            .reader        = _wav_reader_datapcm8
        },{
            .bitspersample = 16,
            .reader        = _wav_reader_datapcm16
        },{
            .bitspersample = 24,
            .reader        = _wav_reader_datapcm24
        },{
            .bitspersample = 32,
            .reader        = _wav_reader_datapcm32
        },
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
            "Unsupported WAV bits per sample value for PCM: %" PRIu16 "\n",
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
