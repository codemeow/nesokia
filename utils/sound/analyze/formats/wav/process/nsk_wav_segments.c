#include "nsk_wav_segments.h"

#include "segments/nsk_wav_sg_pipeline.h"

/*!
 * \brief  Processes the wav segments
 *
 * \param[in]      wav   Source WAV data
 * \param[in,out]  ctx   Processing context whose segments are processed
 * \return True if the processing completed successfully
 */
bool nsk_wav_segment(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx
) {
    return nsk_wav_sg_build_marks(wav, ctx);
}
