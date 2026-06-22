#include <math.h>

#include <nsk_util_meta.h>

#include "nsk_wav_bf_grid.h"
#include "../../../../arguments/nsk_args_options.h"

/*!
 * \brief  Calculates RMS for a sample window
 *
 * Energy-rise detection compares two adjacent windows around a grid frame.
 * This helper measures the energy of one such window without copying samples.
 *
 * \param[in] samples  Window start
 * \param[in] count    Number of samples in the window
 * \return    RMS value for the window
 */
static double _bf_grid_rms(
    const double *samples,
    size_t count
) {
    double sum = 0.0;

    for (size_t i = 0; i < count; i++) {
        sum += samples[i] * samples[i];
    }

    return sqrt(sum / count);
}

/*!
 * \brief  Adds a grid-locked energy-rise candidate
 *
 * A candidate is emitted only when the right-side RMS is higher than the
 * left-side RMS by at least the configured threshold.  The timestamp is locked
 * to the target frame grid instead of the raw sample position.
 *
 * \param[in,out] wav    The wav
 * \param[in]     frame  Target grid frame
 * \param[in]     rise   Energy increase across the frame
 * \return        True if processing can continue
 */
static bool _bf_grid_addenergyrise(
    struct nsk_wav *wav,
    size_t          frame,
    double          rise
) {
    if (rise < nsk_options_program.profile.boundary.grid.energyrisethreshold) {
        return true;
    }

    return nsk_wav_candidate(
        wav,
        (struct nsk_wav_candidate) {
            .method = NSK_WAV_CND_METHOD_GRID_ENERGY_RISE,
            .kind   = NSK_WAV_CND_KIND_GRID_ENERGY_RISE,
            .timestamp =
                (double)frame / nsk_options_program.profile.boundary.grid.fps,
            .strength = rise * nsk_options_program.profile.boundary.grid.strengthscale,
            .confidence = NSK_MIN(
                1.0,
                rise / nsk_options_program.profile.boundary.grid.energyrisethreshold
            )
        }
    );
}

/*!
 * \brief  Finds grid-locked energy-rise boundary candidates
 *
 * For every internal target frame, the detector compares RMS energy in one
 * frame-sized window before the grid time with RMS energy in one frame-sized
 * window after it.  A sufficiently strong positive rise becomes a weak onset
 * candidate placed exactly on the frame grid.
 *
 * \param[in,out]  wav   The wav
 * \return True if processed successfully
 */
bool nsk_wav_bf_grid(
    struct nsk_wav *wav
) {
    const size_t samplesperframe = NSK_MAX(
        (size_t)round(
            (double)wav->format.samplerate /
            nsk_options_program.profile.boundary.grid.fps
        ),
        1
    );

    const size_t framemax = floor(
        (double)wav->samples.raw.count *
        nsk_options_program.profile.boundary.grid.fps /
        wav->format.samplerate
    );

    for (size_t frame = 1; frame < framemax; frame++) {
        const size_t center = round(
            (double)frame *
            wav->format.samplerate /
            nsk_options_program.profile.boundary.grid.fps
        );

        const size_t startleft =
            center > samplesperframe ?
            center - samplesperframe :
            0;

        const size_t endright = NSK_MIN(
            wav->samples.raw.count,
            center + samplesperframe
        );

        const size_t countleft  = center - startleft;
        const size_t countright = endright - center;

        if (countleft == 0 || countright == 0) {
            continue;
        }

        const double rmsleft = _bf_grid_rms(
            &wav->samples.raw.value[startleft],
            countleft
        );

        const double rmsright = _bf_grid_rms(
            &wav->samples.raw.value[center],
            countright
        );

        const double rise = rmsright - rmsleft;

        if (!_bf_grid_addenergyrise(
            wav,
            frame,
            rise
        )) {
            return false;
        }
    }

    return true;
}
