#include <limits.h>
#include <inttypes.h>
#include <stdint.h>

#include <nsk_util_meta.h>

#include "nsk_wav_chunkfmt.h"

#include "../nsk_wav_readers.h"

/*!
 * \brief  Reads the audio format from WAV file
 *
 * \param[in,out]  file  The file descriptor
 * \param[in,out]  wav   The wav data
 * \return True if the data read successfully
 */
static bool _wav_reader_fmtaudioformat(
    FILE           *file,
    struct nsk_wav *wav
) {
    uint16_t af = 0;
    if (!nsk_wav_read_u16le(file, &af)) {
        return false;
    }

    wav->format.audioformat = af;

    return true;
}

/*!
 * \brief  Reads the number of channels from WAV file
 *
 * \param[in,out]  file  The file descriptor
 * \param[in,out]  wav   The wav data
 * \return True if the data read successfully
 */
static bool _wav_reader_fmtchannels(
    FILE           *file,
    struct nsk_wav *wav
) {
    if (!nsk_wav_read_u16le(file, &wav->format.channels)) {
        return false;
    }

    if (wav->format.channels == 0) {
        nsk_err("Corrupted WAV file: channels count cannot be 0\n");
        return false;
    }

    return true;
}

/*!
 * \brief  Reads the sample rate from WAV file
 *
 * \param[in,out]  file  The file descriptor
 * \param[in,out]  wav   The wav data
 * \return True if the data read successfully
 */
static bool _wav_reader_fmtsamplerate(
    FILE           *file,
    struct nsk_wav *wav
) {
    if (!nsk_wav_read_u32le(file, &wav->format.samplerate)) {
        return false;
    }

    if (wav->format.samplerate == 0) {
        nsk_err("Corrupted WAV file: sample rate cannot be 0\n");
        return false;
    }

    return true;
}

/*!
 * \brief  Reads the byte rate from WAV file
 *
 * \param[in,out]  file  The file descriptor
 * \param[in,out]  wav   The wav data
 * \return True if the data read successfully
 */
static bool _wav_reader_fmtbyterate(
    FILE           *file,
    struct nsk_wav *wav
) {
    if (!nsk_wav_read_u32le(file, &wav->format.byterate)) {
        return false;
    }

    return true;
}

/*!
 * \brief  Reads the block alignment from WAV file
 *
 * \param[in,out]  file  The file descriptor
 * \param[in,out]  wav   The wav data
 * \return True if the data read successfully
 */
static bool _wav_reader_fmtblockalign(
    FILE           *file,
    struct nsk_wav *wav
) {
    if (!nsk_wav_read_u16le(file, &wav->format.blockalign)) {
        return false;
    }

    if (wav->format.blockalign == 0) {
        nsk_err("Corrupted WAV file: block align cannot be 0\n");
        return false;
    }

    return true;
}

/*!
 * \brief  Reads bits per sample value from WAV file
 *
 * \param[in,out]  file  The file descriptor
 * \param[in,out]  wav   The wav data
 * \return True if the data read successfully
 */
static bool _wav_reader_fmtbitspersample(
    FILE           *file,
    struct nsk_wav *wav
) {
    if (!nsk_wav_read_u16le(file, &wav->format.bitspersample)) {
        return false;
    }

    if (wav->format.bitspersample % CHAR_BIT != 0) {
        nsk_err(
            "Corrupted WAV file: bits per sample is not a multiple of %d\n",
            (int)CHAR_BIT
        );
        return false;
    }

    return true;
}

/*!
 * \brief  Reads one WAV chunk of type FMT
 *
 * \param[in,out] file          The file descriptor
 * \param[in,out] wav           The wav data
 * \param[in]     chunksize     The expected size of the chunk
 * \param[in,out] rest          The rest number of chunks bytes to read
 * \return True if the chunk successfully read
 */
bool nsk_wav_reader_fmt(
    FILE           *file,
    struct nsk_wav *wav,
    ssize_t         chunksize,
    ssize_t        *rest
) {
    static const struct {
        bool (*reader)(
            FILE           *file,
            struct nsk_wav *wav
        );
        const char *name;
        size_t      size;
    } _table[] = {
        {
            .reader = _wav_reader_fmtaudioformat,
            .name   = "audio format",
            .size   = sizeof(uint16_t)
        },
        {
            .reader = _wav_reader_fmtchannels,
            .name   = "audio channels",
            .size   = sizeof(wav->format.channels)
        },
        {
            .reader = _wav_reader_fmtsamplerate,
            .name   = "sample rate",
            .size   = sizeof(wav->format.samplerate)
        },
        {
            .reader = _wav_reader_fmtbyterate,
            .name   = "byte rate",
            .size   = sizeof(wav->format.byterate)
        },
        {
            .reader = _wav_reader_fmtblockalign,
            .name   = "block align",
            .size   = sizeof(wav->format.blockalign)
        },
        {
            .reader = _wav_reader_fmtbitspersample,
            .name   = "bits per sample",
            .size   = sizeof(wav->format.bitspersample)
        }
    };

    if (chunksize < 16) {
        nsk_err("Malformed WAV `fmt` chunk (less than 16 bytes)\n");
        return false;
    }

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (!_table[i].reader(file, wav)) {
            nsk_err(
                "Cannot read WAV %s\n",
                _table[i].name
            );
            return false;
        }

        chunksize -= _table[i].size;
        if (chunksize < 0) {
            nsk_err("Malformed WAV `fmt` chunk\n");
            return false;
        }

        *rest     -= _table[i].size;
        if (*rest < 0) {
            nsk_err("Corrupted WAV file\n");
            return false;
        }
    }

    if (
        wav->format.byterate !=
            wav->format.samplerate *
            wav->format.blockalign
    ) {
        nsk_err(
            "WAV format mismatch: byte rate (%" PRIu32 ") "
                "does not equal of a multiple of sample rate (%" PRIu32 ") "
                "and block align (%" PRIu16 ")",
            wav->format.byterate,
            wav->format.samplerate,
            wav->format.blockalign
        );
        return false;
    }

    if (
        wav->format.blockalign !=
            wav->format.channels *
            wav->format.bitspersample / CHAR_BIT
    ) {
        nsk_err(
            "WAV format mismatch: block align (%" PRIu16 ") "
                "does not equal of a multiple of channels (%" PRIu16 ") "
                "and bytes per sample (%" PRIu16 " / %d)",
            wav->format.blockalign,
            wav->format.channels,
            wav->format.bitspersample,
            (int)CHAR_BIT
        );
        return false;
    }

    if (chunksize > 0) {
        if (fseek(file, chunksize, SEEK_CUR) != 0) {
            nsk_err("Failed to skip additional WAV FMT data\n");
            return false;
        }
        *rest -= chunksize;
        if (*rest < 0) {
            nsk_err("Corrupted WAV file\n");
            return false;
        }

        nsk_inf(
            "Additional %zu bytes of WAV FMT chunk data skipped\n",
            chunksize
        );
    }

    return true;
}