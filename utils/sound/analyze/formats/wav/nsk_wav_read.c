#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>
#include <sys/types.h>
#include <stdbool.h>
#include <math.h>

#include <nsk_util_meta.h>
#include "nsk_wav_read.h"
#include "nsk_wav_utils.h"
#include "read/nsk_wav_header.h"
#include "read/nsk_wav_readers.h"

#include "read/chunks/nsk_wav_chunkfmt.h"
#include "read/chunks/nsk_wav_chunkdata.h"
#include "read/chunks/nsk_wav_chunklist.h"

/*!
 * \brief  Reads one WAV data chunk
 *
 * \param[in,out] file  The file descriptor
 * \param[in,out] wav   The wav data
 * \param[in,out] rest  The rest number of bytes to read
 * \return True if the chunk successfully read
 */
static bool _wav_read_chunk(
    FILE           *file,
    struct nsk_wav *wav,
    ssize_t        *rest
) {
    typedef bool (*_reader)(
        FILE           *file,
        struct nsk_wav *wav,
        ssize_t         chunksize,
        ssize_t        *rest
    );

    static const struct {
        const uint8_t name[4];
        _reader       reader;
    } _table[] = {
        {
            .name   = { 'f', 'm', 't', ' ' },
            .reader = nsk_wav_reader_fmt
        },
        {
            .name   = { 'd', 'a', 't', 'a' },
            .reader = nsk_wav_reader_data
        },
        {
            .name   = { 'L', 'I', 'S', 'T' },
            .reader = nsk_wav_reader_list
        }
    };

    _reader reader = NULL;
    uint8_t  name[4] = { 0 };
    uint32_t chunksize = 0;

    if (fread(name, sizeof(name), 1, file) != 1) {
        nsk_err("Cannot read WAV chunk name at %ld\n", ftell(file));
        return false;
    }
    *rest -= sizeof(name);

    if (!nsk_wav_read_u32le(file, &chunksize)) {
        nsk_err("Cannot read WAV chunk size at %ld\n", ftell(file));
        return false;
    }
    *rest -= sizeof(chunksize);

    if ((ssize_t)chunksize > *rest) {
        nsk_err("Corrupted WAV file: chunk size exceeds RIFF size\n");
        return false;
    }

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (memcmp(_table[i].name, name, sizeof(name)) == 0) {
            reader = _table[i].reader;
            break;
        }
    }

    if (reader) {
        if (!reader(
                file,
                wav,
                chunksize,
                rest
        )) {
            return false;
        }

    } else {
        if (fseek(file, chunksize, SEEK_CUR) != 0) {
            nsk_err(
                "Cannot skip unknown WAV chunk\n"
            );
            return false;
        }
        *rest -= chunksize;

        nsk_inf(
            "Unknown WAV chunk type: \"%c%c%c%c\"\n",
            name[0],
            name[1],
            name[2],
            name[3]
        );
    }

    /* RIFF chunks are always 2-byte aligned
     * even if the chunk size is odd */
    if (chunksize % 2 != 0) {
        if (*rest <= 0) {
            nsk_err("Corrupted WAV file: missing chunk padding\n");
            return false;
        }

        if (fseek(file, 1, SEEK_CUR) != 0) {
            nsk_err("Cannot skip WAV chunk padding\n");
            return false;
        }

        *rest -= 1;
    }

    return true;
}


/*!
 * \brief  Reads the WAV data from the provided file
 *
 * \param[in,out]  file  The file descriptor
 * \return Allocated WAV data
 */
struct nsk_wav *nsk_wav_read(FILE *file) {
    nsk_auto_wav struct nsk_wav *wav = calloc(1, sizeof(*wav));
    if (!wav) {
        nsk_err("Cannot allocate memory for WAV data\n");
        return NULL;
    }

    if (!nsk_wav_read_header(file, wav)) {
        return NULL;
    }

    ssize_t rest = wav->chunks.size;
    while (rest > 0) {
        if (!_wav_read_chunk(file, wav, &rest)) {
            nsk_err("Cannot read WAV chunk\n");
            return NULL;
        }
    }

    if (rest != 0) {
        nsk_err(
            "Corrupted WAV file, expected %zu data bytes, but read %zd\n",
            wav->chunks.size,
            wav->chunks.size - rest
        );
        return NULL;
    }

    if (wav->samples.raw.count == 0) {
        nsk_err(
            "WAV file does not contain any samples data\n"
        );
        return NULL;
    }

    return nsk_steal(wav);
}