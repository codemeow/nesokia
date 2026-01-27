#include <getopt.h>
#include <nsk_util_meta.h>

#include "../arguments/nsk_args_options.h"
#include "../arguments/processors/nsk_option_help.h"
#include "../arguments/processors/nsk_option_quiet.h"
#include "../arguments/processors/nsk_option_version.h"

/*!
 * Available options table
 */
struct nsk_options_entry nsk_options_table[] = {

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

