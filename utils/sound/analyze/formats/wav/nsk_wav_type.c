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

/*!
 * \brief  Creates empty list of candidates
 *
 * \return Empty allocated list
 */
struct nsk_wav_cnds *nsk_wav_cnds_alloc(void) {
    struct nsk_wav_cnds *cnds = calloc(sizeof(*cnds), 1);
    if (!cnds) {
        nsk_err("Cannot allocate memory for the candidates list\n");
        return NULL;
    }
    return cnds;
}

/*!
 * \brief  Appends new candidate
 *
 * \param[in,out] cnds       List of candidates
 * \param[in]     candidate  The candidate data
 * \return True if successfully appended
 */
bool nsk_wav_cnds_new(
    struct nsk_wav_cnds *cnds,
    struct nsk_wav_cnd candidate
) {
    struct nsk_wav_cnd *ptr = realloc(
        cnds->candidate,
        sizeof(*ptr) * (cnds->count + 1)
    );
    if (!ptr) {
        nsk_err(
            "Cannot realloc the candidates list"
        );
        return false;
    }

    cnds->candidate = ptr;
    cnds->candidate[cnds->count++] = candidate;

    return true;
}

/*!
 * \brief  Frees the WAV candidates data
 *
 * \param[in,out]  cnds  The WAV candidates data
 */
void nsk_wav_cnds_free(struct nsk_wav_cnds *cnds) {
    free(cnds->candidate);
    free(cnds);
}