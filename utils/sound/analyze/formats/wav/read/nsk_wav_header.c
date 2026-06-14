#include <nsk_util_meta.h>

#include "nsk_wav_header.h"
#include "nsk_wav_readers.h"

/*!
 * \brief  Reads WAV file header
 *
 * \param[in,out]  file  The file descriptor
 * \param[in,out]  wav   The wav data
 * \return  True if the header is successfully read
 */
bool nsk_wav_read_header(
    FILE           *file,
    struct nsk_wav *wav
) {
    const uint8_t _head_riff[] = { 'R', 'I', 'F', 'F' };
    const uint8_t _head_wave[] = { 'W', 'A', 'V', 'E' };

    uint32_t riffsize = 0;

    if (
        !nsk_wav_read_compare(file, sizeof(_head_riff), _head_riff) ||
        !nsk_wav_read_u32le  (file, &riffsize) ||
        !nsk_wav_read_compare(file, sizeof(_head_wave), _head_wave)
    ) {
        nsk_err("Cannot read WAV header\n");
        return false;
    }

    if (riffsize < sizeof(_head_wave)) {
        nsk_err("Malformed WAV RIFF size\n");
        return false;
    }

    wav->riff.size   = riffsize;
    wav->chunks.size = riffsize - sizeof(_head_wave);

    return true;
}