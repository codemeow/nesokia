#include <string.h>
#include <nsk_util_meta.h>

#include "../process/nsk_process_output.h"
#include "../arguments/nsk_args_options.h"
#include "../process/nsk_process_vars.h"

/*!
 * \brief  Deduces the output format
 */
static void _output_format_assign(void) {
    if (nsk_options_program.output.format != NSK_OPTION_OUTPUT_AUTO) {
        return;
    }

    if (!nsk_options_program.output.file) {
        nsk_err(
            "No output file provided\n"
            "See `--help` for options\n"
        );
        exit(EXIT_FAILURE);
    }

    const char *extname = strrchr(nsk_options_program.output.file, '.');
    if (!extname) {
        nsk_err(
            "Cannot deduce the output format from filename: \"%s\"\n",
            nsk_options_program.output.file
        );
        exit(EXIT_FAILURE);
    }
    extname++;

    enum nsk_option_outputs format = nsk_output_format(extname);
    if (format == NSK_OPTION_OUTPUT_AUTO) {
        nsk_err(
            "Unknown output file extension (\"%s\") and "
            "`-O`/`--output-format` option is not provided\n",
            extname
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.output.format = format;
}

static void _ppucolors_savepng(
    const char *filename,
    const struct nsk_type_ppucolors *colors
) {
    nsk_auto_pifree struct nsk_type_pngimage *image = nsk_ppucolors_convtopng(
        colors
    );
    nsk_pngimage_write(
        image,
        filename
    );
}

static void _output_file_write(void) {
    static const struct {
        enum nsk_option_outputs format;
        void (*writer)(
            const char *filename,
            const struct nsk_type_ppucolors *colors
        );
    } _table[] = {
        { NSK_OPTION_OUTPUT_ACO, nsk_ppucolors_saveaco },
        { NSK_OPTION_OUTPUT_ASE, nsk_ppucolors_savease },
        { NSK_OPTION_OUTPUT_GPL, nsk_ppucolors_savegpl },
        { NSK_OPTION_OUTPUT_PAL, nsk_ppucolors_savepal },
        { NSK_OPTION_OUTPUT_PNG, _ppucolors_savepng },
    };

    static_assert(
        NSK_SIZE(_table) == NSK_OPTION_OUTPUTS_COUNT - 1,
        "Invalid number of elements in `_output_file_write`"
    );

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (nsk_options_program.output.format == _table[i].format) {
            _table[i].writer(
                nsk_options_program.output.file,
                &nsk_input_ppucolors
            );
            break;
        }
    }
}

/*!
 * \brief  Processes output files
 */
void nsk_process_output(void) {
    _output_format_assign();
    _output_file_write();
}
