#include <nsk_util_meta.h>

#include "nsk_wav_output.h"

/*!
 * \brief  Calculates seconds for a sample index.
 *
 * \param[in] wav     Source WAV data.
 * \param[in] sample  Sample index.
 * \return Time in seconds.
 */
static double _output_seconds(
    const struct nsk_wav *wav,
    size_t sample
) {
    return (double)sample / (double)wav->format.samplerate;
}

/*!
 * \brief  Prints final decoded note/rest events.
 *
 * \param[in]      wav  Source WAV data.
 * \param[in,out]  ctx  Processing context containing decoded events.
 * \return True if decoded events were printed successfully.
 */
bool nsk_wav_output(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    nsk_inf(
        "    %-5s\t%-7s\t%-7s\t%-10s\t%-10s\t%-5s\t%-10s\t%-8s\t%-8s\n",
        "state",
        "start",
        "end",
        "start_sec",
        "end_sec",
        "midi",
        "freq",
        "volume",
        "duty"
    );

    for (size_t i = 0; i < ctx->notes.count; i++) {
        const struct nsk_wav_note *note = &ctx->notes.list[i];

        if (!note->active) {
            nsk_inf(
                "    %-5s\t%-7zu\t%-7zu\t%-10.6f\t%-10.6f\t%-5s\t%-10s\t%-8s\t%-8s\n",
                "rest",
                note->framestart,
                note->frameend,
                _output_seconds(wav, note->samplestart),
                _output_seconds(wav, note->sampleend),
                "",
                "",
                "",
                ""
            );
            continue;
        }

        nsk_inf(
            "    %-5s\t%-7zu\t%-7zu\t%-10.6f\t%-10.6f\t%-5d\t%-10.3f\t%-8.3f\t%-8.3f\n",
            "note",
            note->framestart,
            note->frameend,
            _output_seconds(wav, note->samplestart),
            _output_seconds(wav, note->sampleend),
            note->midi,
            note->frequency,
            note->volume,
            note->duty
        );
    }

    return true;
}
