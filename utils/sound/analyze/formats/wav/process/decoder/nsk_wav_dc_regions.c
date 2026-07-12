#include <stdlib.h>

#include <nsk_util_meta.h>

#include "nsk_wav_dc_regions.h"
#include "../../../../arguments/nsk_args_options.h"

/*!
 * \brief  Appends or merges one local dense region.
 *
 * \param[in,out] regions     Local dense region list to update.
 * \param[in]     framestart  Inclusive region start frame.
 * \param[in]     frameend    Inclusive region end frame.
 * \return True if the region list was updated successfully.
 */
static bool _dc_regions_append(
    struct nsk_wav_dc_regions *regions,
    size_t framestart,
    size_t frameend
) {
    const size_t mergegap =
        nsk_options_program.profile.decoder.localdensemergegap;

    if (
        regions->count > 0 &&
        framestart <= regions->list[regions->count - 1].frameend + mergegap
    ) {
        regions->list[regions->count - 1].frameend = NSK_MAX(
            regions->list[regions->count - 1].frameend,
            frameend
        );
        return true;
    }

    __typeof__(regions->list) ptr = realloc(
        regions->list,
        sizeof(*regions->list) * (regions->count + 1)
    );

    if (!ptr) {
        nsk_err("Cannot allocate local dense decoder regions");
        return false;
    }

    regions->list = ptr;
    regions->list[regions->count++] = (struct nsk_wav_dc_region) {
        .framestart = framestart,
        .frameend = frameend
    };

    return true;
}

/*!
 * \brief  Releases memory owned by a local dense region list.
 *
 * \param[in,out] regions  Local dense region list to clear.
 */
void nsk_wav_dc_regions_free(
    struct nsk_wav_dc_regions *regions
) {
    if (!regions) {
        return;
    }

    free(regions->list);
    *regions = (struct nsk_wav_dc_regions) {0};
}

/*!
 * \brief  Checks whether a frame belongs to a local dense region.
 *
 * \param[in] regions  Local dense regions.
 * \param[in] frame    Frame boundary to test.
 * \return True if the frame is inside any local dense region.
 */
bool nsk_wav_dc_regions_contains(
    const struct nsk_wav_dc_regions *regions,
    size_t frame
) {
    if (!regions) {
        return false;
    }

    for (size_t i = 0; i < regions->count; i++) {
        if (
            frame >= regions->list[i].framestart &&
            frame <= regions->list[i].frameend
        ) {
            return true;
        }
    }

    return false;
}

/*!
 * \brief  Finds local dense regions from strict decoder marks.
 *
 * \param[in]  marks    Decoder mark evidence.
 * \param[out] regions  Local dense regions to fill.
 * \return True if local dense regions were calculated successfully.
 */
bool nsk_wav_dc_regions_find(
    const struct nsk_wav_dc_marks *marks,
    struct nsk_wav_dc_regions *regions
) {
    *regions = (struct nsk_wav_dc_regions) {0};

    if (
        !nsk_options_program.profile.decoder.adaptivedenseenabled ||
        !marks ||
        marks->framecount <= 1
    ) {
        return true;
    }

    const size_t maxframe = marks->framecount - 1;
    size_t strictcount = 0;

    for (size_t frame = 1; frame < maxframe; frame++) {
        if (marks->strictbyframe[frame]) {
            strictcount++;
        }
    }

    if (strictcount == 0) {
        return true;
    }

    nsk_auto_free size_t *strict = calloc(strictcount, sizeof(*strict));

    if (!strict) {
        nsk_err("Cannot allocate strict mark list");
        return false;
    }

    size_t write = 0;

    for (size_t frame = 1; frame < maxframe; frame++) {
        if (marks->strictbyframe[frame]) {
            strict[write++] = frame;
        }
    }

    const size_t mincount =
        nsk_options_program.profile.decoder.localdenseminstrictcount;
    const size_t maxgap =
        nsk_options_program.profile.decoder.localdensemaxgap;
    const double mindensity =
        nsk_options_program.profile.decoder.localdensemindensity;
    const size_t prevextension =
        nsk_options_program.profile.decoder.localdensepreviousextension;
    const size_t nextextension =
        nsk_options_program.profile.decoder.localdensenextextension;

    size_t clusterstart = 0;

    while (clusterstart < strictcount) {
        size_t clusterend = clusterstart + 1;

        while (
            clusterend < strictcount &&
            strict[clusterend] - strict[clusterend - 1] <= maxgap
        ) {
            clusterend++;
        }

        const size_t clustercount = clusterend - clusterstart;

        if (clustercount >= mincount) {
            const size_t clusterspan =
                strict[clusterend - 1] - strict[clusterstart] + 1;
            const double density =
                (double)clustercount / (double)clusterspan;

            if (density >= mindensity) {
                size_t framestart = strict[clusterstart];
                size_t frameend = strict[clusterend - 1];
                const size_t previous =
                    clusterstart > 0 ? strict[clusterstart - 1] : 0;

                if (framestart - previous <= maxgap + prevextension) {
                    framestart = previous;
                }

                size_t nextindex = clusterend;

                while (
                    nextindex < strictcount &&
                    strict[nextindex] <= frameend
                ) {
                    nextindex++;
                }

                while (nextindex < strictcount) {
                    const size_t next = strict[nextindex];

                    if (next - frameend > maxgap + nextextension) {
                        break;
                    }

                    frameend = next;
                    nextindex++;
                }

                framestart = NSK_MIN(framestart, maxframe);
                frameend = NSK_MIN(frameend, maxframe);

                if (
                    frameend > framestart &&
                    frameend - framestart >= mincount &&
                    !_dc_regions_append(regions, framestart, frameend)
                ) {
                    nsk_wav_dc_regions_free(regions);
                    return false;
                }
            }
        }

        clusterstart = clusterend;
    }

    return true;
}
