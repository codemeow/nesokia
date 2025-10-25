#ifndef NSK_TABLE_RAW
#define NSK_TABLE_RAW

#include "../../../types/header/nsk_header_raw.h"

#include "../../../types/header/nsk_header_data.h"

/*!
 * \brief  Converts NES 2.0 signature
 *
 * \param[in] raw   The raw
 * \param     data  The data */
void nsk_convraw_isnes20(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts PRG ROM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_prgrom(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts CHR ROM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_chrrom(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts Miscellaneous ROMs
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_miscroms(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts PRG RAM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_prgram(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts CHR RAM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_chrram(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts PRG NVRAM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_prgnvram(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts CHR NVRAM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_chrnvram(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts mapper id
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_mapper(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts submapper id
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_submapper(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts nametable's layout
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_mirroring(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts alternative nametables flag
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_alternative(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts the trainer
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_trainer(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts battery
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_battery(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts console type
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_console(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts region/timing
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_region(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts expansion device type
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_device(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts Vs. PPU type
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_vsppu(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

/*!
 * \brief  Converts Vs. hardware type
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_vshardware(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
);

#endif