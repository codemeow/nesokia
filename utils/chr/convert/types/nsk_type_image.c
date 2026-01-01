#include <stddef.h>
#include <string.h>
#include <nsk_util_meta.h>

#include "../types/nsk_type_image.h"

#include "../arguments/nsk_args_options.h"
#include "../png/nsk_png_read.h"
#include "../process/nsk_process_vars.h"

/*!
 * \brief  The sizes of the image/image components
 */
enum nsk_image_size {
    /* Simple picture       */
    NSK_IMAGESIZE_FULL_WIDTH        = 328, /*!< Width of the full image     */
    NSK_IMAGESIZE_FULL_HEIGHT       = 248, /*!< Height of the full image    */

    /* Composite picture    */
    /*! \brief Template width       */
    NSK_IMAGESIZE_TEMPLATE_WIDTH    = NSK_IMAGESIZE_FULL_WIDTH,
    /*! \brief Template height      */
    NSK_IMAGESIZE_TEMPLATE_HEIGHT   = NSK_IMAGESIZE_FULL_HEIGHT,
    NSK_IMAGESIZE_LEFT_WIDTH        = 152, /*!< Left nametable width        */
    NSK_IMAGESIZE_LEFT_HEIGHT       = 160, /*!< Left nametbale height       */
    NSK_IMAGESIZE_RIGHT_WIDTH       = 152, /*!< Right nametable width       */
    NSK_IMAGESIZE_RIGHT_HEIGHT      = 160, /*!< Right nametable height      */
    NSK_IMAGESIZE_COLORS_WIDTH      = 152, /*!< Colors width                */
    NSK_IMAGESIZE_COLORS_HEIGHT     = 64,  /*!< Colors height               */
    NSK_IMAGESIZE_PALETTES_WIDTH    = 152, /*!< Palettes width              */
    NSK_IMAGESIZE_PALETTES_HEIGHT   = 64   /*!< Palettes height             */
};

/*!
 * \brief  The positioning for the composite mode
 */
enum nsk_image_position {
    NSK_IMAGEPOS_TEMPLATE_X = 0,    /*!< Template X pos         */
    NSK_IMAGEPOS_TEMPLATE_Y = 0,    /*!< Template Y pos         */
    NSK_IMAGEPOS_LEFT_X     = 8,    /*!< Left nametable X pos   */
    NSK_IMAGEPOS_LEFT_Y     = 8,    /*!< Left nametable Y pos   */
    NSK_IMAGEPOS_RIGHT_X    = 168,  /*!< Right nametable X pos  */
    NSK_IMAGEPOS_RIGHT_Y    = 8,    /*!< Right nametable Y pos  */
    NSK_IMAGEPOS_COLORS_X   = 8,    /*!< Global palette X pos   */
    NSK_IMAGEPOS_COLORS_Y   = 176,  /*!< Global palette Y pos   */
    NSK_IMAGEPOS_PALETTES_X = 168,  /*!< Local palettes X pos   */
    NSK_IMAGEPOS_PALETTES_Y = 176,  /*!< Local palettes Y pos   */
};

/*!
 * \brief  Creates image in local format
 *
 * \param[in] filename  Original filename
 * \param[in] png_ptr   The pnglib main image pointer
 * \param[in] info_ptr  The pnglib info image pointer
 * \return Allocated image
 */
struct nsk_type_image *nsk_image_create(
    const char *filename,
    png_structp png_ptr,
    png_infop info_ptr
) {
    png_byte **bytes = png_get_rows(png_ptr, info_ptr);

    struct nsk_type_image *image = nsk_util_malloc(sizeof(*image));
    image->width    = png_get_image_width (png_ptr, info_ptr);
    image->height   = png_get_image_height(png_ptr, info_ptr);
    image->filename = nsk_strings_strdup(filename);
    image->data     = nsk_util_malloc(sizeof(*image->data) * image->height);

    for (size_t h = 0; h < image->height; h++) {
        image->data[h] = nsk_util_malloc(sizeof(**image->data) * image->width);
        memcpy(image->data[h], bytes[h], image->width * sizeof(**image->data));
    }

    return image;
}

/*!
 * \brief  Creates empty image
 *
 * \param[in] width   The width
 * \param[in] height  The height
 * \return Allocated image
 */
struct nsk_type_image *nsk_image_empty(size_t width, size_t height) {
    struct nsk_type_image *image = nsk_util_malloc(sizeof(*image));
    image->width  = width;
    image->height = height;
    image->filename = nsk_strings_strdup("//Empty");
    image->data   = nsk_util_malloc(sizeof(*image->data) * image->height);

    for (size_t h = 0; h < image->height; h++) {
        image->data[h] = nsk_util_malloc(sizeof(**image->data) * image->width);
        memset(image->data[h], 0xff, image->width * sizeof(**image->data));
    }
    return image;
}

/*!
 * \brief  Checks if the images matches provided sizes
 *
 * \param[in] image   The image
 * \param[in] width   The width
 * \param[in] height  The height
 */
void nsk_image_measure(struct nsk_type_image *image, size_t width, size_t height) {
    if (image->width  != width ||
        image->height != height) {

        nsk_err(
            "Image \"%s\" doesn't match required size: "
                "should be %zux%zu instead of %zux%zu\n",
            image->filename,
            width,
            height,
            image->width,
            image->height
        );
        exit(EXIT_FAILURE);
    }
}

/*!
 * \brief  Combines the target and component images at X,Y
 *
 * \param[in, out] target  The target
 * \param[in] component    The component
 * \param[in] x            X position
 * \param[in] y            Y position
 */
void nsk_image_combine(
    struct nsk_type_image       *target,
    const struct nsk_type_image *component,
    size_t x,
    size_t y
) {
    for (size_t iy = 0; iy < component->height; iy++) {
        for (size_t ix = 0; ix < component->width; ix++) {
            target->data[y + iy][x + ix].raw = component->data[iy][ix].raw;
        }
    }
}

/*!
 * \brief  Frees the nsk_image data
 *
 * \param[in, out]  image
 */
void nsk_image_free(struct nsk_type_image *image) {
    if (!image) {
        return;
    }

    for (size_t h = 0; h < image->height; h++) {
        nsk_util_free(image->data[h]);
    }
    nsk_util_free(image->data);
    nsk_util_free(image->filename);
    nsk_util_free(image);
}

/*!
 * \brief  Validate the simple input files combinations
 */
static void _input_simple_validate(void) {
    if (nsk_options_program.files[1]) {
        nsk_err(
            "Only one input image is allowed\n"
        );
        exit(EXIT_FAILURE);
    }

    if (nsk_options_program.input.template ||
        nsk_options_program.input.left     ||
        nsk_options_program.input.right    ||
        nsk_options_program.input.colors   ||
        nsk_options_program.input.palettes) {

        nsk_err(
            "Simple input and composite input cannot be mixed\n"
        );
        exit(EXIT_FAILURE);
    }
}

/*!
 * \brief  Reads the image in the simple mode
 */
static void _input_simple_read(void) {
    nsk_inf("# Simple mode\n");
    nsk_input.image = nsk_png_read(nsk_options_program.files[0]);
    nsk_image_measure(
        nsk_input.image,
        NSK_IMAGESIZE_FULL_WIDTH,
        NSK_IMAGESIZE_FULL_HEIGHT
    );
    nsk_inf("  - Loaded image: \"%s\"\n", nsk_options_program.files[0]);
    nsk_inf("\n");
}

/*!
 * \brief  Validate the simple input files combinations
 */
static void _input_composite_validate(void) {
    if (!nsk_options_program.input.template) {

        if (!(nsk_options_program.input.left   &&
              nsk_options_program.input.right  &&
              nsk_options_program.input.colors &&
              nsk_options_program.input.palettes)
            ) {

            nsk_err(
                "Template image must be passed if one or more composite "
                    "components are missing\n"
            );
            exit(EXIT_FAILURE);
        }
    }
}

/*!
 * \brief  Reads the image in the composite mode
 */
static void _input_composite_read(void) {
    const struct {
        const char *filename;
        const char *name;
        size_t      width;
        size_t      height;
        size_t      x;
        size_t      y;
    } _table[] = {
        {
            .filename   = nsk_options_program.input.template,
            .name       = "template",
            .width      = NSK_IMAGESIZE_TEMPLATE_WIDTH,
            .height     = NSK_IMAGESIZE_TEMPLATE_HEIGHT,
            .x          = NSK_IMAGEPOS_TEMPLATE_X,
            .y          = NSK_IMAGEPOS_TEMPLATE_Y
        },
        {
            .filename   = nsk_options_program.input.colors,
            .name       = "colors",
            .width      = NSK_IMAGESIZE_COLORS_WIDTH,
            .height     = NSK_IMAGESIZE_COLORS_HEIGHT,
            .x          = NSK_IMAGEPOS_COLORS_X,
            .y          = NSK_IMAGEPOS_COLORS_Y
        },
        {
            .filename   = nsk_options_program.input.palettes,
            .name       = "palettes",
            .width      = NSK_IMAGESIZE_PALETTES_WIDTH,
            .height     = NSK_IMAGESIZE_PALETTES_HEIGHT,
            .x          = NSK_IMAGEPOS_PALETTES_X,
            .y          = NSK_IMAGEPOS_PALETTES_Y
        },
        {
            .filename   = nsk_options_program.input.left,
            .name       = "left table",
            .width      = NSK_IMAGESIZE_LEFT_WIDTH,
            .height     = NSK_IMAGESIZE_LEFT_HEIGHT,
            .x          = NSK_IMAGEPOS_LEFT_X,
            .y          = NSK_IMAGEPOS_LEFT_Y
        },
        {
            .filename   = nsk_options_program.input.right,
            .name       = "right table",
            .width      = NSK_IMAGESIZE_RIGHT_WIDTH,
            .height     = NSK_IMAGESIZE_RIGHT_HEIGHT,
            .x          = NSK_IMAGEPOS_RIGHT_X,
            .y          = NSK_IMAGEPOS_RIGHT_Y
        },
    };

    nsk_inf("# Composite mode\n");

    nsk_input.image = nsk_image_empty(
        NSK_IMAGESIZE_TEMPLATE_WIDTH,
        NSK_IMAGESIZE_TEMPLATE_HEIGHT
    );

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        nsk_inf("  * Component: %s\n", _table[i].name);

        if (!_table[i].filename) {
            nsk_inf("    - Skipped\n");
            continue;
        }

        struct nsk_type_image *component = nsk_png_read(_table[i].filename);
        nsk_image_measure(component, _table[i].width, _table[i].height);
        nsk_image_combine(
            nsk_input.image,
            component,
            _table[i].x,
            _table[i].y
        );

        nsk_inf("    - Loaded image: \"%s\"\n", _table[i].filename);
    }
    nsk_inf("\n");
}

/*!
 * \brief  Reads the image from the input file/files
 */
void nsk_image_read(void) {
    if (nsk_options_program.files[0]) {
        _input_simple_validate();
        _input_simple_read();

    } else {
        _input_composite_validate();
        _input_composite_read();
    }
}

/*!
 * \brief  Validates the image
 */
void nsk_image_validate(void) {
    /*
     * No checks at this point. Invalid image cannot be created
     */
}