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

    free(wav->candidates.candidate);
    free(wav->spans.span);
    free(wav->edges.rise.edge);
    free(wav->edges.fall.edge);

    free(wav);
}

/*!
 * \brief  Appends new candidate
 *
 * \param[in,out] wav        The wav
 * \param[in]     candidate  The candidate data
 * \return True if successfully appended
 */
bool nsk_wav_candidate(
    struct nsk_wav          *wav,
    struct nsk_wav_candidate candidate
) {
    __typeof__(wav->candidates.candidate) ptr = realloc(
        wav->candidates.candidate,
        sizeof(*ptr) * (wav->candidates.count + 1)
    );
    if (!ptr) {
        nsk_err(
            "Cannot realloc the candidates list"
        );
        return false;
    }

    wav->candidates.candidate = ptr;
    wav->candidates.candidate[wav->candidates.count++] = candidate;

    return true;
}

/*!
 * \brief  Appends new detected energy span
 *
 * \param[in,out] wav   The wav
 * \param[in]     span  The span
 * \return  True if successfully appended
 */
bool nsk_wav_span(
    struct nsk_wav     *wav,
    struct nsk_wav_span span
) {
    __typeof__(wav->spans.span) ptr = realloc(
        wav->spans.span,
        sizeof(*ptr) * (wav->spans.count + 1)
    );
    if (!ptr) {
        nsk_err(
            "Cannot realloc the span list"
        );
        return false;
    }

    wav->spans.span = ptr;
    wav->spans.span[wav->spans.count++] = span;

    return true;
}

/*!
 * \brief  Appends new detected egde
 *
 * \param[in,out]  edge       The edge
 * \param[in]      timestamp  The timestamp
 * \return True if successfully appended
 */
bool nsk_wav_edge(
    struct nsk_wav_edges *edge,
    double                timestamp
) {
    __typeof__(edge->edge) ptr = realloc(
        edge->edge,
        sizeof(*ptr) * (edge->count + 1)
    );

    if (!ptr) {
        nsk_err(
            "Cannot realloc the edge list"
        );
        return false;
    }

    edge->edge = ptr;
    edge->edge[edge->count++] = timestamp;

    return true;
}
