#include <nsk_util_meta.h>

#include "nsk_wav_chunkdata.h"
#include "data/nsk_wav_datapcm.h"
#include "data/nsk_wav_datafloat.h"

/*!
 * \brief  Reads one WAV chunk of type DATA
 *
 * \param[in,out] file          The file descriptor
 * \param[in,out] wav           The wav data
 * \param[in]     chunksize     The expected size of the chunk
 * \param[in,out] rest          The rest number of chunks bytes to read
 */
bool nsk_wav_reader_data(
    FILE           *file,
    struct nsk_wav *wav,
    ssize_t         chunksize,
    ssize_t        *rest
) {
    static const struct {
        enum nsk_wav_audioformat format;
        bool (*reader)(
            FILE           *file,
            struct nsk_wav *wav,
            ssize_t         chunksize,
            ssize_t        *rest
        );
    } _table[] = {
        {
            .format = NSK_WAV_FORMAT_PCM,
            .reader = nsk_wav_reader_datapcm
        },
        {
            .format = NSK_WAV_FORMAT_IEEE_FLOAT,
            .reader = nsk_wav_reader_datafloat
        }
    };

    if (wav->format.channels == 0) {
        nsk_err("WAV section `data` before `fmt` is unsupported\n");
        return false;
    }

    if (wav->samples.raw.count != 0) {
        nsk_err("Multiple `data` sections in WAV are not supported\n");
        return false;
    }

    if (chunksize == 0) {
        wav->samples.raw.count = 0;
        wav->samples.raw.value = NULL;
        nsk_inf("WAV `data` section does not contain any data\n");
        return true;
    }

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (_table[i].format == wav->format.audioformat) {
            return _table[i].reader(
                file,
                wav,
                chunksize,
                rest
            );
        }
    }

    nsk_err("Unsupported WAV audio format: %d\n", (int)wav->format.audioformat);
    return false;
}