#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include <nsk_util_meta.h>

#include "nsk_wav_chunklist.h"
#include "list/nsk_wav_listinfo.h"

/*!
 * \brief  Reads one WAV chunk of type LIST
 *
 * \param[in,out] file          The file descriptor
 * \param[in,out] wav           The wav data
 * \param[in]     chunksize     The expected size of the chunk
 * \param[in,out] rest          The rest number of chunks bytes to read
 * \return True if the chunk successfully read
 */
bool nsk_wav_reader_list(
    FILE           *file,
    struct nsk_wav *wav,
    ssize_t         chunksize,
    ssize_t        *rest
) {
    typedef bool (*_reader)(
        FILE           *file,
        struct nsk_wav *wav,
        ssize_t        *list_rest
    );

    static const struct {
        const uint8_t name[4];
        _reader       reader;
    } _table[] = {
        {
            .name   = { 'I', 'N', 'F', 'O' },
            .reader = nsk_wav_reader_listinfo
        }
    };

    if (chunksize < 4) {
        nsk_err("Corrupted WAV file: invalid LIST chunk\n");
        return false;
    }

    _reader reader = NULL;
    uint8_t name[4] = { 0 };

    if (fread(name, sizeof(name), 1, file) != 1) {
        nsk_err("Cannot read WAV LIST chunk header at %ld\n", ftell(file));
        return false;
    }
    *rest -= sizeof(name);

    ssize_t list_rest = chunksize - (ssize_t)sizeof(name);

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (memcmp(_table[i].name, name, sizeof(name)) == 0) {
            reader = _table[i].reader;
            break;
        }
    }

    if (reader) {
        if (!reader(file, wav, &list_rest)) {
            return false;
        }

    } else {
        if (fseek(file, list_rest, SEEK_CUR) != 0) {
            nsk_err("Cannot skip unknown WAV LIST content\n");
            return false;
        }
        list_rest = 0;

        nsk_inf(
            "Unknown WAV LIST subtype: \"%c%c%c%c\"\n",
            name[0],
            name[1],
            name[2],
            name[3]
        );
    }

    if (list_rest != 0) {
        nsk_err("Corrupted WAV file: LIST chunk was not fully consumed\n");
        return false;
    }

    *rest -= chunksize - (ssize_t)sizeof(name);

    return true;
}
