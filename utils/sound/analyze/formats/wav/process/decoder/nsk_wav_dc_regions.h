#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "nsk_wav_dc_marks.h"

/*!
 * \brief  Local dense decoder frame region.
 */
struct nsk_wav_dc_region {
    size_t framestart; /*!< Inclusive region start frame */
    size_t frameend;   /*!< Inclusive region end frame */
};

/*!
 * \brief  Local dense decoder region list.
 */
struct nsk_wav_dc_regions {
    size_t count; /*!< Number of local dense regions */
    struct nsk_wav_dc_region *list; /*!< Local dense region list */
};

/*!
 * \brief  Releases memory owned by a local dense region list.
 *
 * \param[in,out] regions  Local dense region list to clear.
 */
void nsk_wav_dc_regions_free(
    struct nsk_wav_dc_regions *regions
);

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
);

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
);
