#include <getopt.h>
#include <string.h>
#include <assert.h>
#include <nsk_util_meta.h>

#include "../arguments/nsk_args_options.h"
#include "../arguments/processors/nsk_option_input.h"
#include "../arguments/processors/nsk_option_input_format.h"
#include "../arguments/processors/nsk_option_output.h"
#include "../arguments/processors/nsk_option_output_format.h"
#include "../arguments/processors/nsk_option_help.h"
#include "../arguments/processors/nsk_option_quiet.h"
#include "../arguments/processors/nsk_option_version.h"

/*!
 * Available options table
 */
struct nsk_options_entry nsk_options_table[] = {
    /* Input options */
    {
        "input", 'i', required_argument,
        nsk_option_input,
        "Provides input file.\n"
        "\n"
        "Notes\n"
        "    * By default the input format is deduced from the file extension.\n"
        "    * This behavior can be overridden with the `-I` option.\n"
        "\n"
    },
    {
        "input-format", 'I', required_argument,
        nsk_option_input_format,
        "Forces input file format.\n"
        "\n"
        "Format\n"
        "    Supported input formats:\n"
        "     - png\n"
        "     - pal\n"
        "\n"
        "Notes\n"
        "    * Overrides automatic format detection based on file extension.\n"
        "\n"
    },

    /* Output options */
    {
        "output", 'o', required_argument,
        nsk_option_output,
        "Provides output file.\n"
        "\n"
        "Notes\n"
        "    * By default the output format is deduced from the file extension.\n"
        "    * This behavior can be overridden with the `-O` option.\n"
        "\n"
    },
    {
        "output-format", 'O', required_argument,
        nsk_option_output_format,
        "Forces output file format.\n"
        "\n"
        "Format\n"
        "    Supported output formats:\n"
        "     - png\n"
        "     - pal\n"
        "     - gpl\n"
        "     - ase\n"
        "     - aco\n"
        "\n"
        "Notes\n"
        "    * Overrides automatic format detection based on file extension.\n"
        "\n"
    },

    /* Common options */
    {
        "quiet", 'q', no_argument,
        nsk_option_quiet,
        "Suppress any output but errors\n"
        "\n"
    },
    {
        "help", 'h', no_argument,
        nsk_option_help,
        "Print help and exit\n"
        "\n"
        "Shows usage and option reference.\n"
        "\n"
    },
    {
        "version", 'v', no_argument,
        nsk_option_version,
        "Print program version and exit\n"
        "\n"
    }
};

/*!
 * Available options table size
 */
size_t nsk_options_count = NSK_SIZE(nsk_options_table);

/*!
 * \brief  Provided program options
 */
struct nsk_options_program nsk_options_program = { 0 };

/*!
 * \brief  Provides the input format by string definition
 *
 * \param[in] string  The string
 * \return format or _AUTO
 */
enum nsk_option_inputs nsk_input_format(const char *string) {
    static const struct {
        enum nsk_option_inputs format;
        const char *string;
    } _table[] = {
        { NSK_OPTION_INPUT_PAL, "pal" },
        { NSK_OPTION_INPUT_PNG, "png" },
    };

    static_assert(
        NSK_SIZE(_table) == NSK_OPTION_INPUTS_COUNT - 1,
        "Invalid number of elements in `nsk_input_format`"
    );

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (strcmp(string, _table[i].string) == 0) {
            return _table[i].format;
        }
    }

    return NSK_OPTION_INPUT_AUTO;
}

/*!
 * \brief  Provides the output format by string definition
 *
 * \param[in] string  The string
 * \return format or _AUTO
 */
enum nsk_option_outputs nsk_output_format(const char *string) {
    static const struct {
        enum nsk_option_outputs format;
        const char *string;
    } _table[] = {
        { NSK_OPTION_OUTPUT_ACO, "aco" },
        { NSK_OPTION_OUTPUT_ASE, "ase" },
        { NSK_OPTION_OUTPUT_GPL, "gpl" },
        { NSK_OPTION_OUTPUT_PAL, "pal" },
        { NSK_OPTION_OUTPUT_PNG, "png" },
    };

    static_assert(
        NSK_SIZE(_table) == NSK_OPTION_OUTPUTS_COUNT - 1,
        "Invalid number of elements in `nsk_output_format`"
    );

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (strcmp(string, _table[i].string) == 0) {
            return _table[i].format;
        }
    }

    return NSK_OPTION_OUTPUT_AUTO;
}
