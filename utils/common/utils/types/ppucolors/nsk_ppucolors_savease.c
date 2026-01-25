#include <threads.h>
#include <string.h>
#include <stdlib.h>

#include "../../types/ppucolors/nsk_ppucolors_savease.h"
#include "../../log/nsk_log_err.h"
#include "../../nsk_util_cleanup.h"
#include "../../types/ppucolors/nsk_ppucolors_common.h"
#include "../../math/nsk_math_endianness.h"

/*!
 * \brief  Number of static strings available in one function call
 */
#define _STATIC_CAROUSEL_SIZE (5)

/*!
 * \brief  ASE block types
 */
enum nsk_formatase_blocktype {
    NSK_FORMATASE_BLOCKTYPE_COLOR  = 0x0001, /*!< Color entry   */
    NSK_FORMATASE_BLOCKTYPE_GOPEN  = 0xc001, /*!< Group opening */
    NSK_FORMATASE_BLOCKTYPE_GCLOSE = 0xc002  /*!< Group closing */
};

/*!
 * \brief  ASE color models
 */
enum nsk_formatase_colormodel {
    NSK_FORMATASE_COLORMODEL_RGB   = 0x52474220, /*!< "RGB " */
    NSK_FORMATASE_COLORMODEL_CMYK  = 0x434d594b, /*!< "CMYK" */
    NSK_FORMATASE_COLORMODEL_LAB   = 0x4c414220, /*!< "LAB " */
    NSK_FORMATASE_COLORMODEL_GRAY  = 0x47726179, /*!< "Gray" */
};

/*!
 * \brief ASE color types
 */
enum nsk_formatase_colortype {
    NSK_FORMATASE_COLORTYPE_GLOBAL = 0,
    NSK_FORMATASE_COLORTYPE_SPOT   = 1,
    NSK_FORMATASE_COLORTYPE_NORMAL = 2
};

/*!
 * \brief  Converts name value to UTF16-BE format
 *
 * \warning Only supports ASCII-characters (codes < 128)
 *
 * \param[in]  name  The ASCII name
 * \param[out] size  The result size in bytes
 * \return Static zero-terminated UTF16-BE "string"
 */
static const uint8_t *_name_toUTF16(const char *name, uint16_t *size) {
    static thread_local uint8_t string[_STATIC_CAROUSEL_SIZE][1024];
    static thread_local size_t index;

    if (++index >= _STATIC_CAROUSEL_SIZE) {
        index = 0;
    }

    size_t namelen = strlen(name);
    if ((namelen * 2) >= sizeof(string[0])) {
        nsk_err(
            "UTF16 name buffer: overflow"
        );
        abort();
    }

    *size = (namelen + 1);
    for (size_t i = 0; i < namelen; i++) {
        uint8_t c = name[i];
        if (c >= 0x80) {
            nsk_err(
                "UTF16 name buffer: unsupported character"
            );
            abort();
        }
        string[index][i * 2 + 0] = 0;
        string[index][i * 2 + 1] = c;
    }

    string[index][2 * namelen + 0] = 0;
    string[index][2 * namelen + 1] = 1;

    return string[index];
}

/*!
 * \brief  Saves ASE signature to the file
 *
 * \param[in,out]  file      The file
 * \param[in]      filename  The filename
 */
static void _savease_magic(
    FILE *file,
    const char *filename
) {
    static const uint32_t magic = 0x46455341; /* "ASEF" */
    nsk_ppucolors_fwrite(&magic, sizeof(magic), file, filename);
}

/*!
 * \brief  Saves ASE file version
 *
 * \param[in,out]  file      The file
 * \param[in]      filename  The filename
 */
static void _savease_version(
    FILE *file,
    const char *filename
) {
    static const uint16_t major = 1;
    static const uint16_t minor = 0;
    uint16_t buf;

    buf = nsk_math_tobe16(major);
    nsk_ppucolors_fwrite(&buf, sizeof(buf), file, filename);
    buf = nsk_math_tobe16(minor);
    nsk_ppucolors_fwrite(&buf, sizeof(buf), file, filename);
}

/*!
 * \brief  Saves ASE blocks count
 *
 * \param[in,out]  file      The file
 * \param[in]      filename  The filename
 * \param[in]      colors    The colors
 */
static void _savease_blockcount(
    FILE *file,
    const char *filename,
    const struct nsk_type_ppucolors *colors
) {
    uint32_t count = 0;
    for (size_t i = 0; i < NSK_PPUCOLORSTABLE_COUNT; i++) {
        if (colors->allowed[i]) {
            count++;
        }
    }

    count = nsk_math_tobe32(count);
    nsk_ppucolors_fwrite(&count, sizeof(count), file, filename);
}

/*!
 * \brief  Saves block name
 *
 * \param[in,out] file      The file
 * \param[in]     filename  The filename
 * \param[in]     index     The index
 */
static void _savease_blockname(
    FILE *file,
    const char *filename,
    size_t index
) {
    char name[128];
    snprintf(
        name,
        sizeof(name),
        "$%02zx",
        index
    );
    /* Note: After changing the name change the block length */

    uint16_t utf16len;
    const uint8_t *utf16name = _name_toUTF16(name, &utf16len);
    uint16_t utf16lenbe = nsk_math_tobe16(utf16len);

    nsk_ppucolors_fwrite(
        &utf16lenbe,
        sizeof(utf16lenbe),
        file,
        filename
    );
    nsk_ppucolors_fwrite(
        utf16name,
        utf16len * sizeof(uint16_t),
        file,
        filename
    );
}

/*!
 * \brief  Saves block name
 *
 * \param[in,out] file      The file
 * \param[in]     filename  The filename
 */
static void _savease_blockmodel(
    FILE *file,
    const char *filename
) {
    uint32_t model = nsk_math_tobe32(NSK_FORMATASE_COLORMODEL_RGB);
    nsk_ppucolors_fwrite(
        &model,
        sizeof(model),
        file,
        filename
    );
}

/*!
 * \brief  Saves block name
 *
 * \param[in,out] file      The file
 * \param[in]     filename  The filename
 * \param[in]     color     The color
 */
static void _savease_blockcolor(
    FILE *file,
    const char *filename,
    const union nsk_type_color4 *color
) {
    float r = nsk_math_tobeflt(color->r / 255.0);
    float g = nsk_math_tobeflt(color->g / 255.0);
    float b = nsk_math_tobeflt(color->b / 255.0);
    nsk_ppucolors_fwrite(&r, sizeof(r), file, filename);
    nsk_ppucolors_fwrite(&g, sizeof(g), file, filename);
    nsk_ppucolors_fwrite(&b, sizeof(b), file, filename);
}

/*!
 * \brief  Saves block type
 *
 * \param[in,out] file      The file
 * \param[in]     filename  The filename
 */
static void _savease_blocktype(
    FILE *file,
    const char *filename
) {
    uint16_t type = nsk_math_tobe16(NSK_FORMATASE_COLORTYPE_GLOBAL);
    nsk_ppucolors_fwrite(&type, sizeof(type), file, filename);
}

/*!
 * \brief  Saves block kind
 *
 * \param[in,out] file      The file
 * \param[in]     filename  The filename
 */
static void _savease_blockkind(
    FILE *file,
    const char *filename
) {
    uint16_t kind = nsk_math_tobe16(NSK_FORMATASE_BLOCKTYPE_COLOR);
    nsk_ppucolors_fwrite(&kind, sizeof(kind), file, filename);
}

/*!
 * \brief  Saves block length
 *
 * \param[in,out] file      The file
 * \param[in]     filename  The filename
 */
static void _savease_blocklength(
    FILE *file,
    const char *filename
) {
    static const uint32_t blocklen = 28;

    uint32_t length = nsk_math_tobe32(blocklen);
    nsk_ppucolors_fwrite(&length, sizeof(length), file, filename);
}

/*!
 * \brief  Saves ASE blocks data
 *
 * \param[in,out]  file      The file
 * \param[in]      filename  The filename
 * \param[in]      colors    The colors
 */
static void _savease_blockdata(
    FILE *file,
    const char *filename,
    const struct nsk_type_ppucolors *colors
) {
    for (size_t i = 0; i < NSK_PPUCOLORSTABLE_COUNT; i++) {
        if (!colors->allowed[i]) {
            continue;
        }

        _savease_blockkind  (file, filename);
        _savease_blocklength(file, filename);
        _savease_blockname  (file, filename, i);
        _savease_blockmodel (file, filename);
        _savease_blockcolor (file, filename, &colors->colors[i]);
        _savease_blocktype  (file, filename);
    }
}

/*!
 * \brief  Saves the PPU colors as .ase (Adobe Swatch Exchange) file
 *
 * \param[in] filename  The filename
 * \param[in] colors    The colors
 */
void nsk_ppucolors_savease(
    const char *filename,
    const struct nsk_type_ppucolors *colors
) {
    nsk_auto_fclose FILE *file = nsk_ppucolors_fopen(filename, "wb");

    _savease_magic     (file, filename);
    _savease_version   (file, filename);
    _savease_blockcount(file, filename, colors);
    _savease_blockdata (file, filename, colors);
}
