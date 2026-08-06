#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nsk_util_meta.h>

#include "../arguments/nsk_args_options.h"
#include "../process/nsk_process_object.h"

enum nsk_object_size {
    NSK_OBJECTSIZE_TILEGROUP = 2,
    NSK_OBJECTSIZE_PIXELS_MIN = NSK_TILESIZE_WIDTH * NSK_OBJECTSIZE_TILEGROUP,
};

/*!
 * \brief  Checks whether a color occurs in a palette group
 */
static bool _palette_contains(
    const struct nsk_type_palette *palette,
    size_t                         group,
    union nsk_type_color4          color
) {
    for (size_t i = 0; i < NSK_PALETTESIZE_COLORS; i++) {
        if (palette->group[group].color[i].raw == color.raw) {
            return true;
        }
    }

    return false;
}

/*!
 * \brief  Checks if a 2x2 background group matches a palette group
 */
static bool _group_matches(
    const struct nsk_type_pngimage *image,
    const struct nsk_type_palette  *palette,
    size_t                          palette_group,
    size_t                          startx,
    size_t                          starty
) {
    for (size_t y = 0; y < NSK_OBJECTSIZE_PIXELS_MIN; y++) {
        for (size_t x = 0; x < NSK_OBJECTSIZE_PIXELS_MIN; x++) {
            if (!_palette_contains(
                palette,
                palette_group,
                image->data[starty + y][startx + x]
            )) {
                return false;
            }
        }
    }

    return true;
}

/*!
 * \brief  Selects the first matching palette for one 2x2 tile group
 */
static bool _group_selectpalette(
    const struct nsk_type_pngimage *image,
    const struct nsk_type_palette  *palette,
    size_t                          groupx,
    size_t                          groupy,
    size_t                         *selected
) {
    const size_t startx = groupx * NSK_OBJECTSIZE_PIXELS_MIN;
    const size_t starty = groupy * NSK_OBJECTSIZE_PIXELS_MIN;

    for (size_t group = 0; group < NSK_PALETTESIZE_GROUPS; group++) {
        if (_group_matches(
            image,
            palette,
            group,
            startx,
            starty
        )) {
            *selected = group;
            return true;
        }
    }

    nsk_err(
        "Cannot find a matching palette for object group [%zu, %zu]\n",
        groupx,
        groupy
    );
    return false;
}

/*!
 * \brief  Validates object mode CLI combinations
 */
static void _options_validate(void) {
    if (!nsk_options_program.input.ppucolors) {
        nsk_err("Invalid input: no PPU colors PNG template is provided\n");
        exit(EXIT_FAILURE);
    }

    if ((nsk_options_program.input.palettes.both == NULL) ==
        (nsk_options_program.input.palettes.back == NULL)) {
        nsk_err(
            "Invalid input: object mode requires exactly one of combined "
            "or background palettes\n"
        );
        exit(EXIT_FAILURE);
    }

    if (nsk_options_program.input.full ||
        nsk_options_program.input.palettes.sprites ||
        nsk_options_program.input.pattables.both ||
        nsk_options_program.input.pattables.left ||
        nsk_options_program.input.pattables.right
    ) {
        nsk_err(
            "Invalid input: object PNG cannot be combined with template or "
            "pattern table inputs\n"
        );
        exit(EXIT_FAILURE);
    }

    if (!nsk_options_program.output.object ||
        !nsk_options_program.output.object_attributes
    ) {
        nsk_err(
            "Invalid output: object mode requires both --output-object and "
            "--output-object-attributes\n"
        );
        exit(EXIT_FAILURE);
    }

    if (nsk_options_program.output.full ||
        nsk_options_program.output.ppucolors ||
        nsk_options_program.output.palettes.both ||
        nsk_options_program.output.palettes.back ||
        nsk_options_program.output.palettes.sprites ||
        nsk_options_program.output.pattables.both ||
        nsk_options_program.output.pattables.left ||
        nsk_options_program.output.pattables.right
    ) {
        nsk_err(
            "Invalid output: object output cannot be combined with template "
            "component outputs\n"
        );
        exit(EXIT_FAILURE);
    }

    if (nsk_options_program.ppuctrl34_set) {
        nsk_err("Invalid option: --back-address cannot be used in object mode\n");
        exit(EXIT_FAILURE);
    }

    if (nsk_options_program.input.explicit) {
        nsk_err("Invalid option: --explicit-palettes cannot be used in object mode\n");
        exit(EXIT_FAILURE);
    }
}

/*!
 * \brief  Loads and validates the palette needed by a background PNG
 */
static void _palette_load(
    struct nsk_type_ppucolors *ppucolors,
    struct nsk_type_palette   *palette
) {
    if (!nsk_ppucolors_readpng(nsk_options_program.input.ppucolors, ppucolors)) {
        exit(EXIT_FAILURE);
    }

    if (!nsk_ppucolors_validate(ppucolors)) {
        exit(EXIT_FAILURE);
    }

    if (nsk_options_program.input.palettes.both) {
        struct nsk_type_palettes palettes = { 0 };
        if (!nsk_palettes_readpng(
            nsk_options_program.input.palettes.both,
            &palettes
        )) {
            exit(EXIT_FAILURE);
        }
        *palette = palettes.plane[NSK_PLANE_BACKGROUND];
        if (!nsk_palette_setindexes(ppucolors, palette)) {
            exit(EXIT_FAILURE);
        }

    } else {
        if (!nsk_palette_readspal(
            nsk_options_program.input.palettes.back,
            palette
        ) || !nsk_palette_setcolors(ppucolors, palette)) {
            exit(EXIT_FAILURE);
        }
    }

    if (!nsk_palette_validate(ppucolors, palette)) {
        exit(EXIT_FAILURE);
    }
}

/*!
 * \brief  Saves background tiles as raw row-major CHR data
 */
static bool _tiles_save(
    const char                 *filename,
    const struct nsk_type_tile *tiles,
    size_t                      count
) {
    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "wb");
    if (!file) {
        nsk_err("Error: cannot open file \"%s\" for background tile writing\n", filename);
        return false;
    }

    for (size_t i = 0; i < count; i++) {
        if (!nsk_tile_savechr(&tiles[i], file, filename)) {
            return false;
        }
    }

    return true;
}

/*!
 * \brief  Saves background palette selectors as raw attribute bytes
 */
static bool _attributes_save(
    const char    *filename,
    const uint8_t *attributes,
    size_t         count
) {
    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "wb");
    if (!file) {
        nsk_err("Error: cannot open file \"%s\" for background attribute writing\n", filename);
        return false;
    }

    if (fwrite(attributes, count, 1, file) != 1) {
        nsk_err("Error: cannot write background attributes to \"%s\"\n", filename);
        return false;
    }

    return true;
}

/*!
 * \brief  Processes a standalone background object PNG in t2c mode
 */
void nsk_process_object(void) {
    _options_validate();

    struct nsk_type_ppucolors ppucolors = { 0 };
    struct nsk_type_palette palette = { 0 };
    _palette_load(&ppucolors, &palette);

    nsk_auto_pifree struct nsk_type_pngimage *image = nsk_pngimage_read(
        nsk_options_program.input.object
    );
    if (!image) {
        exit(EXIT_FAILURE);
    }

    if (image->width < NSK_OBJECTSIZE_PIXELS_MIN ||
        image->height < NSK_OBJECTSIZE_PIXELS_MIN ||
        image->width % NSK_OBJECTSIZE_PIXELS_MIN != 0 ||
        image->height % NSK_OBJECTSIZE_PIXELS_MIN != 0
    ) {
        nsk_err(
            "Object PNG dimensions must be multiples of %d pixels\n",
            NSK_OBJECTSIZE_PIXELS_MIN
        );
        exit(EXIT_FAILURE);
    }

    const size_t tiles_width = image->width / NSK_TILESIZE_WIDTH;
    const size_t tiles_height = image->height / NSK_TILESIZE_HEIGHT;
    const size_t groups_width = tiles_width / NSK_OBJECTSIZE_TILEGROUP;
    const size_t groups_height = tiles_height / NSK_OBJECTSIZE_TILEGROUP;
    const size_t attributes_width = (groups_width + 1) / 2;
    const size_t attributes_height = (groups_height + 1) / 2;

    nsk_auto_free struct nsk_type_tile *tiles = nsk_util_malloc(
        sizeof(*tiles) * tiles_width * tiles_height
    );
    nsk_auto_free uint8_t *attributes = nsk_util_malloc(
        sizeof(*attributes) * attributes_width * attributes_height
    );
    memset(attributes, 0, attributes_width * attributes_height);

    for (size_t groupy = 0; groupy < groups_height; groupy++) {
        for (size_t groupx = 0; groupx < groups_width; groupx++) {
            size_t selected;
            if (!_group_selectpalette(
                image,
                &palette,
                groupx,
                groupy,
                &selected
            )) {
                exit(EXIT_FAILURE);
            }

            const size_t attribute =
                (groupy / 2) * attributes_width + groupx / 2;
            const unsigned shift =
                (unsigned)((groupy % 2) * 4 + (groupx % 2) * 2);
            attributes[attribute] |= (uint8_t)(selected << shift);

            for (size_t tiley = 0; tiley < NSK_OBJECTSIZE_TILEGROUP; tiley++) {
                for (size_t tilex = 0; tilex < NSK_OBJECTSIZE_TILEGROUP; tilex++) {
                    const size_t x = groupx * NSK_OBJECTSIZE_TILEGROUP + tilex;
                    const size_t y = groupy * NSK_OBJECTSIZE_TILEGROUP + tiley;
                    struct nsk_type_tile *tile = &tiles[y * tiles_width + x];

                    nsk_tile_readpng(
                        image,
                        x * NSK_TILESIZE_WIDTH,
                        y * NSK_TILESIZE_HEIGHT,
                        tile
                    );
                    tile->palette = selected;
                    tile->init.palette = true;
                    if (!nsk_tile_setindex(tile, &palette)) {
                        nsk_err(
                            "Cannot encode object tile [%zu, %zu]\n",
                            x,
                            y
                        );
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }

    if (!_tiles_save(
        nsk_options_program.output.object,
        tiles,
        tiles_width * tiles_height
    ) || !_attributes_save(
        nsk_options_program.output.object_attributes,
        attributes,
        attributes_width * attributes_height
    )) {
        exit(EXIT_FAILURE);
    }
}
