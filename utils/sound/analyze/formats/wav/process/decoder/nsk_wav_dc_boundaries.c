#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_dc_boundaries.h"
#include "../../../../arguments/nsk_args_options.h"

/*!
 * \brief  Finds nearest marked frame in a by-frame lookup array.
 *
 * \param[in] values  By-frame lookup array
 * \param[in] count   Number of lookup entries
 * \param[in] frame   Frame boundary index to test
 * \param[in] limit   Maximum distance to search
 * \return Nearest distance, or limit + 1 if no mark is nearby
 */
static size_t _boundary_nearest_distance(
    const bool *values,
    size_t count,
    size_t frame,
    size_t limit
) {
    if (frame < count && values[frame]) {
        return 0;
    }

    for (size_t distance = 1; distance <= limit; distance++) {
        if (
            frame >= distance &&
            values[frame - distance]
        ) {
            return distance;
        }

        if (
            frame + distance < count &&
            values[frame + distance]
        ) {
            return distance;
        }
    }

    return limit + 1;
}

/*!
 * \brief  Releases memory owned by boundary costs.
 *
 * \param[in,out] boundaries  Boundary cost table to clear
 */
void nsk_wav_dc_boundaries_free(
    struct nsk_wav_dc_boundaries *boundaries
) {
    if (!boundaries) {
        return;
    }

    free(boundaries->costs);
    *boundaries = (struct nsk_wav_dc_boundaries) {0};
}

/*!
 * \brief  Returns a boundary cost.
 *
 * \param[in] boundaries  Boundary cost table
 * \param[in] frame       Frame boundary index
 * \return Boundary cost, or 0 when frame is outside the table
 */
double nsk_wav_dc_boundaries_cost(
    const struct nsk_wav_dc_boundaries *boundaries,
    size_t frame
) {
    if (!boundaries || frame >= boundaries->count) {
        return 0.0;
    }

    return boundaries->costs[frame];
}

/*!
 * \brief  Builds boundary costs from decoder mark evidence.
 *
 * \param[in]  marks       Decoder marks and by-frame lookup arrays
 * \param[out] boundaries  Boundary cost table to fill
 * \return True if boundary costs were built successfully
 */
bool nsk_wav_dc_boundaries_build(
    const struct nsk_wav_dc_marks *marks,
    struct nsk_wav_dc_boundaries *boundaries
) {
    *boundaries = (struct nsk_wav_dc_boundaries) {0};

    if (!marks || marks->framecount == 0) {
        return true;
    }

    boundaries->count = marks->framecount;
    boundaries->costs = calloc(
        boundaries->count,
        sizeof(*boundaries->costs)
    );

    if (!boundaries->costs) {
        nsk_err("Cannot allocate decoder boundary costs");
        return false;
    }

    const size_t maxframe = boundaries->count - 1;
    const size_t radius =
        nsk_options_program.profile.decoder.boundarynearradius;

    for (size_t frame = 0; frame < boundaries->count; frame++) {
        if (frame == 0 || frame == maxframe) {
            boundaries->costs[frame] = 0.0;
            continue;
        }

        const size_t strictdistance = _boundary_nearest_distance(
            marks->strictbyframe,
            marks->framecount,
            frame,
            radius
        );
        const size_t markdistance = _boundary_nearest_distance(
            marks->presentbyframe,
            marks->framecount,
            frame,
            radius
        );

        if (strictdistance == 0) {
            boundaries->costs[frame] = 0.0;
        } else if (markdistance == 0) {
            boundaries->costs[frame] =
                nsk_options_program.profile.decoder.snappenalty;
        } else if (strictdistance <= radius) {
            boundaries->costs[frame] =
                nsk_options_program.profile.decoder.snappenalty *
                (1.0 + strictdistance);
        } else if (markdistance <= radius) {
            boundaries->costs[frame] =
                nsk_options_program.profile.decoder.snappenalty *
                (2.0 + markdistance);
        } else {
            boundaries->costs[frame] =
                nsk_options_program.profile.decoder.nonmarkboundarypenalty;
        }
    }

    return true;
}
