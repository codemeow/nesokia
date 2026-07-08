#include <stdio.h>
#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_type.h"


/*!
 * \brief  Converts the enum value of the format to constant string
 *
 * \param[in] format  The format
 * \return  Constant string
 */
const char *nsk_wav_aftostring(enum nsk_wav_audioformat format) {
    switch (format) {
        case NSK_WAV_FORMAT_UNKNOWN     : return "Unknown";
        case NSK_WAV_FORMAT_PCM         : return "PCM";
        case NSK_WAV_FORMAT_ADPCM       : return "ADPCM";
        case NSK_WAV_FORMAT_IEEE_FLOAT  : return "IEEE float";
        case NSK_WAV_FORMAT_ALAW        : return "ALAW";
        case NSK_WAV_FORMAT_MULAW       : return "MULAW";
        case NSK_WAV_FORMAT_EXTENSIBLE  : return "Extensible";
    }

    static __thread char buf[30];
    snprintf(
        buf,
        sizeof(buf),
        "Unknown format: %d",
        (int)format
    );
    return buf;
}

/*!
 * \brief  Frees the WAV data
 *
 * \param[in,out] wav  The WAV data
 */
void nsk_wav_free(struct nsk_wav *wav) {
    if (!wav) {
        return;
    }

    free(wav->meta.title);
    free(wav->meta.artist);
    free(wav->meta.comment);
    free(wav->meta.software);
    free(wav->samples.raw.value);

    free(wav);
}
