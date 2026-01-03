#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <threads.h>
#include <nsk_util_meta.h>

#include "../process/nsk_process_output.h"
#include "../arguments/nsk_args_options.h"
#include "../process/nsk_process_vars.h"
#include "../types/nsk_type_nametable.h"
#include "../types/nsk_type_palette.h"
#include "../types/nsk_type_planes.h"

/*!
 * \brief  Number of static strings in functions, returning static strings
 */
#define _STATIC_CAROUSEL_SIZE (5)

/*!
 * \brief List of the items to save
 */
static const struct {
    const char         **filename;      /*!< Filename of the output file    */
    const char          *defname;       /*!< Default name to save           */
    enum nsk_plane_list  plane;         /*!< Object plane                   */
    void                *target;        /*!< The object to save             */
    void               (*converter)(    /*!< Converter function             */
        const char *filename,
        enum nsk_plane_list plane,
        const void *ptr
    );
} _table_output[] = {
    {
        .filename   = &nsk_options_program.output.nametable[NSK_PLANE_BACKGROUND],
        .defname    = "back.chr",
        .plane      = NSK_PLANE_BACKGROUND,
        .target     = &nsk_input.nametables.plane[NSK_PLANE_BACKGROUND],
        .converter  = nsk_nametable_output
    },
    {
        .filename   = &nsk_options_program.output.nametable[NSK_PLANE_SPRITES],
        .defname    = "sprites.chr",
        .plane      = NSK_PLANE_SPRITES,
        .target     = &nsk_input.nametables.plane[NSK_PLANE_SPRITES],
        .converter  = nsk_nametable_output
    },
    {
        .filename   = &nsk_options_program.output.palettes[NSK_PLANE_BACKGROUND],
        .defname    = "back.pal",
        .plane      = NSK_PLANE_BACKGROUND,
        .target     = &nsk_input.palettes.planes[NSK_PLANE_BACKGROUND],
        .converter  = nsk_palette_output
    },
    {
        .filename   = &nsk_options_program.output.palettes[NSK_PLANE_SPRITES],
        .defname    = "sprites.pal",
        .plane      = NSK_PLANE_SPRITES,
        .target     = &nsk_input.palettes.planes[NSK_PLANE_SPRITES],
        .converter  = nsk_palette_output
    }
};

/*!
 * \brief  Sets the default names if no names provided
 */
static void _output_defaults_set(void) {
    for (size_t i = 0; i < NSK_SIZE(_table_output); i++) {
        if (*_table_output[i].filename) {
            return;
        }
    }

    for (size_t i = 0; i < NSK_SIZE(_table_output); i++) {
        *_table_output[i].filename = _table_output[i].defname;
    }
}

/*!
 * \brief  Combines the provided filename and the base directory
 *
 * \param[in] filename  The filename
 * \return Static string
 */
static const char *_filename_combine(const char *filename) {
    NSK_STATIC_ASSERT(
        _STATIC_CAROUSEL_SIZE >=
            (
                NSK_SIZE(nsk_options_program.output.nametable) +
                NSK_SIZE(nsk_options_program.output.palettes)
            ),
        "_STATIC_CAROUSEL_SIZE must be large enough to contain the list of outputs"
    );

    static thread_local char string[_STATIC_CAROUSEL_SIZE][PATH_MAX];
    static thread_local size_t index;

    if (++index >= _STATIC_CAROUSEL_SIZE) {
        index = 0;
    }

    snprintf(
        string[index],
        sizeof(string[index]),
        "%s%c%s",
        nsk_options_program.directory.output,
        nsk_path_delimeter,
        filename
    );

    return string[index];
}

/*!
 * \brief  Compiles the final filenames for the composite parts
 */
static void _output_filenames_set(void) {
    if (!nsk_options_program.directory.output) {
        return;
    }

    for (size_t i = 0; i < NSK_SIZE(_table_output); i++) {
        *_table_output[i].filename =
            _filename_combine(*_table_output[i].filename);
    }
}

/*!
 * \brief  Processes and saves output files
 */
void nsk_process_output(void) {
    _output_defaults_set();
    _output_filenames_set();

    for (size_t i = 0; i < NSK_SIZE(_table_output); i++) {
        if (!*_table_output[i].filename) {
            continue;
        }

        _table_output[i].converter(
            *_table_output[i].filename,
            _table_output[i].plane,
            _table_output[i].target
        );
    }
}
