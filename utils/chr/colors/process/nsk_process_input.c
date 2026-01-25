#include <string.h>
#include <nsk_util_meta.h>

#include "../process/nsk_process_input.h"
#include "../arguments/nsk_args_options.h"
#include "../process/nsk_process_vars.h"

/*!
 * \brief  Deduces the input format
 */
static void _input_format_assign(void) {
    if (nsk_options_program.input.format != NSK_OPTION_INPUT_AUTO) {
        return;
    }

    if (!nsk_options_program.input.file) {
        nsk_err(
            "No input file provided\n"
            "See `--help` for options\n"
        );
        exit(EXIT_FAILURE);
    }

    const char *extname = strrchr(nsk_options_program.input.file, '.');
    if (!extname) {
        nsk_err(
            "Cannot deduce the input format from filename: \"%s\"\n",
            nsk_options_program.input.file
        );
        exit(EXIT_FAILURE);
    }
    extname++;

    enum nsk_option_inputs format = nsk_input_format(extname);
    if (format == NSK_OPTION_INPUT_AUTO) {
        nsk_err(
            "Unknown input file extension (\"%s\") and "
            "`-I`/`--input-format` option is not provided\n",
            extname
        );
        exit(EXIT_FAILURE);
    }

    nsk_options_program.input.format = format;
}

/*!
 * \brief  Reads the PPU colors from the provided file
 */
static void _input_file_read(void) {
    static const struct {
        enum nsk_option_inputs format;
        struct nsk_type_ppucolors (*reader)(const char *filename);
    } _table[] = {
        { NSK_OPTION_INPUT_PAL, nsk_ppucolors_readpal },
        { NSK_OPTION_INPUT_PNG, nsk_ppucolors_readpng }
    };

    static_assert(
        NSK_SIZE(_table) == NSK_OPTION_INPUTS_COUNT - 1,
        "Invalid number of elements in `_input_file_read`"
    );

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (nsk_options_program.input.format == _table[i].format) {
            nsk_input_ppucolors = _table[i].reader(
                nsk_options_program.input.file
            );
            break;
        }
    }
}

/*!
 * \brief  Shows the data from the input file
 */
static void _input_file_show(void) {
    nsk_ppucolors_show(&nsk_input_ppucolors);
}

/*!
 * \brief  Processes input files
 */
void nsk_process_input(void) {
    _input_format_assign();
    _input_file_read();
    _input_file_show();
}
