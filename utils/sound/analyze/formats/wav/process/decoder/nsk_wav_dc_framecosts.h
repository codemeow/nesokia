#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "../types/nsk_wav_ctx_meta.h"
#include "../../nsk_wav_type.h"

/*!
 * \brief  Decoder per-frame state cost table.
 */
struct nsk_wav_dc_framecosts {
    size_t framecount; /*!< Number of grid frames */
    size_t statecount; /*!< Number of decoder states, including rest */

    int midimin; /*!< MIDI note represented by state index 1 */
    int midimax; /*!< Highest represented MIDI note */

    double *costs;  /*!< Cost matrix indexed by state then frame */
    double *prefix; /*!< Prefix sums indexed by state then frame boundary */

    bool *localactive;  /*!< One-frame local activity by frame */
    int *localmidi;     /*!< One-frame local MIDI by frame, or -1 */
    int *periodmidi;    /*!< One-frame period MIDI by frame, or -1 */
    double *periodscore; /*!< One-frame period score by frame */
};

/*!
 * \brief  Releases memory owned by a frame cost table.
 *
 * \param[in,out] costs  Frame cost table to clear
 */
void nsk_wav_dc_framecosts_free(struct nsk_wav_dc_framecosts *costs);

/*!
 * \brief  Returns the MIDI note represented by a decoder state.
 *
 * \param[in] costs  Frame cost table
 * \param[in] state  Decoder state index
 * \return MIDI note for note states, or -1 for rest/invalid states
 */
int nsk_wav_dc_framecosts_midi(
    const struct nsk_wav_dc_framecosts *costs,
    size_t state
);

/*!
 * \brief  Returns one frame cost value.
 *
 * \param[in] costs  Frame cost table
 * \param[in] state  Decoder state index
 * \param[in] frame  Frame index
 * \return Cost value, or 0 when the coordinates are outside the table
 */
double nsk_wav_dc_framecosts_cost(
    const struct nsk_wav_dc_framecosts *costs,
    size_t state,
    size_t frame
);

/*!
 * \brief  Returns an interval cost from prefix sums.
 *
 * \param[in] costs       Frame cost table
 * \param[in] state       Decoder state index
 * \param[in] framestart  Inclusive frame start
 * \param[in] frameend    Exclusive frame end
 * \return Cost value, or 0 when the coordinates are outside the table
 */
double nsk_wav_dc_framecosts_interval(
    const struct nsk_wav_dc_framecosts *costs,
    size_t state,
    size_t framestart,
    size_t frameend
);

/*!
 * \brief  Builds decoder per-frame state costs and prefix sums.
 *
 * \param[in]      wav    Source WAV data
 * \param[in,out]  ctx    Processing context
 * \param[out]     costs  Frame cost table to fill
 * \return True if the table was built successfully
 */
bool nsk_wav_dc_framecosts_build(
    const struct nsk_wav *wav,
    struct nsk_wav_ctx  *ctx,
    struct nsk_wav_dc_framecosts *costs
);
