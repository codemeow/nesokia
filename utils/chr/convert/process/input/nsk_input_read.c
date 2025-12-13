#include <stddef.h>

#include "../../process/input/nsk_input_read.h"

#include "../../process/nsk_process_vars.h"
#include "../../arguments/nsk_args_options.h"
#include "../../png/nsk_png_read.h"
#include "../../types/nsk_type_template.h"
#include "../../utils/nsk_util_size.h"
#include "../../utils/log/nsk_log_inf.h"

/*!
 * \brief  Load input in simple single-file mode
 */
static void _input_simple(void) {
    nsk_inf("# Simple mode\n");
    nsk_image_input = nsk_png_read(nsk_options_program.files[0]);
    nsk_image_measure(
        nsk_image_input,
        NSK_FULL_WIDTH,
        NSK_FULL_HEIGHT
    );
    nsk_inf("  - Loaded image: \"%s\"\n", nsk_options_program.files[0]);
}

/*!
 * \brief  Load input in composite mode
 */
static void _input_composite(void) {
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
            .width      = NSK_TEMPLATE_WIDTH,
            .height     = NSK_TEMPLATE_HEIGHT,
            .x          = 0,
            .y          = 0
        },
        {
            .filename   = nsk_options_program.input.colors,
            .name       = "colors",
            .width      = NSK_COLORS_WIDTH,
            .height     = NSK_COLORS_HEIGHT,
            .x          = NSK_POS_COLORS_X,
            .y          = NSK_POS_COLORS_Y
        },
        {
            .filename   = nsk_options_program.input.palettes,
            .name       = "palettes",
            .width      = NSK_PALETTES_WIDTH,
            .height     = NSK_PALETTES_HEIGHT,
            .x          = NSK_POS_PALETTES_X,
            .y          = NSK_POS_PALETTES_Y
        },
        {
            .filename   = nsk_options_program.input.left,
            .name       = "left table",
            .width      = NSK_LEFT_WIDTH,
            .height     = NSK_LEFT_HEIGHT,
            .x          = NSK_POS_LEFT_X,
            .y          = NSK_POS_LEFT_Y
        },
        {
            .filename   = nsk_options_program.input.right,
            .name       = "right table",
            .width      = NSK_RIGHT_WIDTH,
            .height     = NSK_RIGHT_HEIGHT,
            .x          = NSK_POS_RIGHT_X,
            .y          = NSK_POS_RIGHT_Y
        },
    };

    nsk_inf("# Composite mode\n");

    nsk_image_input = nsk_image_empty(
        NSK_TEMPLATE_WIDTH,
        NSK_TEMPLATE_HEIGHT
    );

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        nsk_inf("  * Component: %s\n", _table[i].name);

        if (!_table[i].filename) {
            nsk_inf("    - Skipped\n");
            continue;
        }

        struct nsk_image *component = nsk_png_read(_table[i].filename);
        nsk_image_measure(
            component,
            _table[i].width,
            _table[i].height
        );

        nsk_image_combine(
            nsk_image_input,
            component,
            _table[i].x,
            _table[i].y
        );

        nsk_inf("    - Loaded image: \"%s\"\n", _table[i].filename);
    }
}

/*!
 * \brief  Read the input file into global byte buffer
 */
void nsk_input_read(void) {
    if (nsk_options_program.files[0]) {
        _input_simple();
    } else {
        _input_composite();
    }
}