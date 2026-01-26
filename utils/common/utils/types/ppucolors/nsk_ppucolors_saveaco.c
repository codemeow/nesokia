#include <threads.h>

#include "../../types/ppucolors/nsk_ppucolors_saveaco.h"
#include "../../nsk_util_cleanup.h"
#include "../../types/ppucolors/nsk_ppucolors_common.h"
#include "../../math/nsk_math_endianness.h"

/*!
 * \brief  ACO color space IDs (per Adobe doc)
 */
enum nsk_formataco_colorspace {
    NSK_FORMATACO_COLORSPACE_RGB = 0, /*!< RGB (0..65535 per channel) */
    /* Others exist, but we only write RGB here */
};

/*!
 * \brief  Saves ACO version (uint16 BE) and count (uint16 BE)
 */
static void _saveaco_header(
    FILE *file,
    const char *filename,
    uint16_t version,
    uint16_t count
) {
    version = nsk_math_tobe16(version);
    count = nsk_math_tobe16(count);
    nsk_ppucolors_fwrite(
        &version,
        sizeof(version),
        file,
        filename
    );
    nsk_ppucolors_fwrite(
        &count,
        sizeof(count),
        file,
        filename
    );
}

/*!
 * \brief  Counts allowed colors
 *
 * \param[in] colors PPU colors
 */
static uint16_t _saveaco_count_allowed(const struct nsk_type_ppucolors *colors) {
    size_t count = 0;
    for (size_t i = 0; i < NSK_PPUCOLORSTABLE_COUNT; i++) {
        if (colors->allowed[i]) {
            count++;
        }
    }
    return count;
}

/*!
 * \brief  Writes one ACO color record
 *
 * \param[in,out]   file      The file
 * \param[in]       filename  The filename
 * \param[in]       color     The color
 */
static void _saveaco_record_rgb(
    FILE *file,
    const char *filename,
    const union nsk_type_color4 *color
) {
    uint16_t space = nsk_math_tobe16(NSK_FORMATACO_COLORSPACE_RGB);

    /* 8-bit [0..255] -> 16-bit [0..65535] */
    uint16_t r = nsk_math_tobe16(color->r * 257);
    uint16_t g = nsk_math_tobe16(color->g * 257);
    uint16_t b = nsk_math_tobe16(color->b * 257);
    uint16_t x = 0; /* unused */

    nsk_ppucolors_fwrite(&space, sizeof(space), file, filename);
    nsk_ppucolors_fwrite(&r,     sizeof(r),     file, filename);
    nsk_ppucolors_fwrite(&g,     sizeof(g),     file, filename);
    nsk_ppucolors_fwrite(&b,     sizeof(b),     file, filename);
    nsk_ppucolors_fwrite(&x,     sizeof(x),     file, filename);
}

/*!
 * \brief  Writes one ACO v2 name
 *
 * \param[in,out]   file        The file
 * \param[in]       filename    The filename
 * \param[in]       name_ascii  The name ascii
 */
static void _saveaco_name_v2(
    FILE *file,
    const char *filename,
    const char *name_ascii
) {
    size_t utf16lenraw;
    const uint8_t *utf16 = nsk_ppucolors_toUTF16be(
        name_ascii,
        &utf16lenraw
    );
    uint32_t utf16len = nsk_math_tobe32(utf16lenraw);

    nsk_ppucolors_fwrite(
        &utf16len,
        sizeof(utf16len),
        file,
        filename
    );
    nsk_ppucolors_fwrite(
        utf16,
        utf16lenraw * sizeof(uint16_t),
        file,
        filename
    );
}

/*!
 * \brief  Writes ACO v1 block (colors only)
 *
 * \param[in,out]   file      The file
 * \param[in]       filename  The filename
 * \param[in]       colors    The colors
 * \param[in]       count     The count
 */
static void _saveaco_v1(
    FILE *file,
    const char *filename,
    const struct nsk_type_ppucolors *colors,
    uint16_t count
) {
    _saveaco_header(file, filename, 1, count);

    for (size_t i = 0; i < NSK_PPUCOLORSTABLE_COUNT; i++) {
        if (!colors->allowed[i]) {
            continue;
        }
        _saveaco_record_rgb(file, filename, &colors->colors[i]);
    }
}

/*!
 * \brief  Writes ACO v2 block (colors + names)
 *
 * \param[in,out]   file      The file
 * \param[in]       filename  The filename
 * \param[in]       colors    The colors
 * \param[in]       count     The count
 */
static void _saveaco_v2(
    FILE *file,
    const char *filename,
    const struct nsk_type_ppucolors *colors,
    uint16_t count
) {
    _saveaco_header(file, filename, 2, count);

    for (size_t i = 0; i < NSK_PPUCOLORSTABLE_COUNT; i++) {
        if (!colors->allowed[i]) {
            continue;
        }

        _saveaco_record_rgb(file, filename, &colors->colors[i]);

        char name[128];
        snprintf(name, sizeof(name), "$%02zx", i);
        _saveaco_name_v2(file, filename, name);
    }
}

/*!
 * \brief  Saves the PPU colors as .aco (Photoshop Color Swatch) file
 *
 * \param[in] filename  The filename
 * \param[in] colors    The colors
 */
void nsk_ppucolors_saveaco(
    const char *filename,
    const struct nsk_type_ppucolors *colors
) {
    nsk_auto_fclose FILE *file = nsk_ppucolors_fopen(filename, "wb");

    uint16_t count = _saveaco_count_allowed(colors);

    _saveaco_v1(file, filename, colors, count);
    _saveaco_v2(file, filename, colors, count);
}
