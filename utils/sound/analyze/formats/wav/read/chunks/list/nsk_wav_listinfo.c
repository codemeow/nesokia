#include <string.h>

#include <nsk_util_meta.h>

#include "nsk_wav_listinfo.h"
#include "../../nsk_wav_readers.h"

/*!
 * \brief  Reads the INFO string chunk
 *
 * \param[in,out] file          The file descriptor
 * \param[in]     chunksize     The expected size of the chunk
 * \param[in,out] rest          The rest number of chunks bytes to read
 * \param[out]    value         The value pointer to save
 */
static bool _wav_reader_infostr(
    FILE           *file,
    ssize_t         chunksize,
    ssize_t        *rest,
    char          **value
) {
    *value = calloc(chunksize + 1, 1);
    if (!*value) {
        nsk_err(
            "Cannot allocate %zd bytes of memory for INFO string\n",
            chunksize
        );
        return false;
    }

    if (fread(*value, chunksize, 1, file) != 1) {
        nsk_err(
            "Cannot read WAV INFO string\n"
        );
        return false;
    }

    *rest -= chunksize;
    return true;
}

/*!
 * \brief  Reads the INFO INAM chunk
 *
 * \param[in,out] file          The file descriptor
 * \param[in,out] wav           The wav data
 * \param[in]     chunksize     The expected size of the chunk
 * \param[in,out] rest          The rest number of chunks bytes to read
 */
static bool _wav_reader_infoinam(
    FILE           *file,
    struct nsk_wav *wav,
    ssize_t         chunksize,
    ssize_t        *rest
) {
    return _wav_reader_infostr(
        file,
        chunksize,
        rest,
        &wav->meta.title
    );
}

/*!
 * \brief  Reads the INFO INAM chunk
 *
 * \param[in,out] file          The file descriptor
 * \param[in,out] wav           The wav data
 * \param[in]     chunksize     The expected size of the chunk
 * \param[in,out] rest          The rest number of chunks bytes to read
 */
static bool _wav_reader_infoiart(
    FILE           *file,
    struct nsk_wav *wav,
    ssize_t         chunksize,
    ssize_t        *rest
) {
    return _wav_reader_infostr(
        file,
        chunksize,
        rest,
        &wav->meta.artist
    );
}

/*!
 * \brief  Reads the INFO INAM chunk
 *
 * \param[in,out] file          The file descriptor
 * \param[in,out] wav           The wav data
 * \param[in]     chunksize     The expected size of the chunk
 * \param[in,out] rest          The rest number of chunks bytes to read
 */
static bool _wav_reader_infoicmt(
    FILE           *file,
    struct nsk_wav *wav,
    ssize_t         chunksize,
    ssize_t        *rest
) {
    return _wav_reader_infostr(
        file,
        chunksize,
        rest,
        &wav->meta.comment
    );
}

/*!
 * \brief  Reads the INFO INAM chunk
 *
 * \param[in,out] file          The file descriptor
 * \param[in,out] wav           The wav data
 * \param[in]     chunksize     The expected size of the chunk
 * \param[in,out] rest          The rest number of chunks bytes to read
 */
static bool _wav_reader_infoisft(
    FILE           *file,
    struct nsk_wav *wav,
    ssize_t         chunksize,
    ssize_t        *rest
) {
    return _wav_reader_infostr(
        file,
        chunksize,
        rest,
        &wav->meta.software
    );
}

/*!
 * \brief  Reads one WAV chunk of type LIST in INFO format
 *
 * \param[in,out] file          The file descriptor
 * \param[in,out] wav           The wav data
 * \param[in,out] rest          The rest number of chunks bytes to read
 */
bool nsk_wav_reader_listinfo(
    FILE           *file,
    struct nsk_wav *wav,
    ssize_t        *rest
) {
    typedef bool (*_reader)(
        FILE           *file,
        struct nsk_wav *wav,
        ssize_t         chunksize,
        ssize_t        *list_rest
    );

    static const struct {
        const uint8_t name[4];
        _reader       reader;
    } _table[] = {
        {
            .name   = { 'I', 'N', 'A', 'M' },
            .reader = _wav_reader_infoinam
        },
        {
            .name   = { 'I', 'A', 'R', 'T' },
            .reader = _wav_reader_infoiart
        },
        {
            .name   = { 'I', 'C', 'M', 'T' },
            .reader = _wav_reader_infoicmt
        },
        {
            .name   = { 'I', 'S', 'F', 'T' },
            .reader = _wav_reader_infoisft
        },
    };

    while (*rest > 0) {
        _reader reader = NULL;
        uint8_t  name[4] = { 0 };
        uint32_t chunksize = 0;

        if (*rest < (ssize_t)(sizeof(name) + sizeof(chunksize))) {
            nsk_err("Corrupted WAV file: incomplete LIST/INFO subchunk header\n");
            return false;
        }

        if (fread(name, sizeof(name), 1, file) != 1) {
            nsk_err("Cannot read WAV INFO subchunk name at %ld\n", ftell(file));
            return false;
        }
        *rest -= sizeof(name);

        if (!nsk_wav_read_u32le(file, &chunksize)) {
            nsk_err("Cannot read WAV INFO subchunk size at %ld\n", ftell(file));
            return false;
        }
        *rest -= sizeof(chunksize);

        if ((ssize_t)chunksize > *rest) {
            nsk_err("Corrupted WAV file: INFO subchunk size exceeds LIST size\n");
            return false;
        }

        for (size_t i = 0; i < NSK_SIZE(_table); i++) {
            if (memcmp(_table[i].name, name, sizeof(name)) == 0) {
                reader = _table[i].reader;
                break;
            }
        }

        if (reader) {
            if (!reader(file, wav, chunksize, rest)) {
                return false;
            }

        } else {
            if (fseek(file, chunksize, SEEK_CUR) != 0) {
                nsk_err("Cannot skip unknown WAV LIST/INFO subchunk\n");
                return false;
            }
            *rest -= chunksize;

            nsk_inf(
                "Unknown WAV INFO subchunk type: \"%c%c%c%c\"\n",
                name[0],
                name[1],
                name[2],
                name[3]
            );
        }

        /* INFO subchunks are word-aligned inside LIST. */
        if (chunksize % 2 != 0) {
            if (*rest <= 0) {
                nsk_err("Corrupted WAV file: missing LIST/INFO subchunk padding\n");
                return false;
            }

            if (fseek(file, 1, SEEK_CUR) != 0) {
                nsk_err("Cannot skip WAV LIST/INFO subchunk padding\n");
                return false;
            }

            *rest -= 1;
        }
    }

    return true;
}