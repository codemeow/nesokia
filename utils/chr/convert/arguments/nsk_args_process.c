#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <nsk_util_meta.h>

#include "../arguments/nsk_args_process.h"

#include "../arguments/nsk_args_options.h"

/*!
 * \brief  Disables system getopt-related messages
 */
static void _getopt_messagesdisable(void) {
    opterr = 0;
}

/*!
 * \brief  Returns the list of short options provided by #nsk_options_table
 *
 * \return Allocated string of options
 */
static char *_options_short(void) {
    /* Worst case - all options are optional, which should be
     * mapped as "X::", thus 3 characters per entry */
    char *options = nsk_util_malloc(
        nsk_options_count * 3 + 1
    );

    size_t pos = 0;
    for (size_t i = 0; i < nsk_options_count; i++) {

        if (nsk_options_table[i].option_short < NSK_OPTION_SHORTLIMIT) {
            options[pos++] = nsk_options_table[i].option_short;

            switch (nsk_options_table[i].option_arg) {
                case no_argument:
                    break;

                case optional_argument:
                    options[pos++] = ':';
                    options[pos++] = ':';
                    break;

                case required_argument:
                    options[pos++] = ':';
                    break;

                default:
                    nsk_err(
                        "Error: bad `option_arg` in %zu-th record\n",
                        i
                    );
                    abort();
            }
        }
    }

    return options;
}

/*!
 * \brief  Return the list of long options provided by #nsk_options_table
 *
 * \return Allocated list of options
 */
static struct option *_options_long(void) {
    struct option *options = nsk_util_malloc(
        sizeof(struct option) * (nsk_options_count + 1)
    );

    for (size_t i = 0; i < nsk_options_count; i++) {
        options[i].name     = nsk_options_table[i].option_long;
        options[i].has_arg  = nsk_options_table[i].option_arg;
        options[i].val      = nsk_options_table[i].option_short;
    }

    return options;
}

/*!
 * \brief  Returns an index of the provided symbol in the #nsk_options_table
 *
 * \param[in] option_short  The short option symbol
 * \return Index of the record in the table
 */
static size_t _option_index(int option_short) {
    for (size_t i = 0; i < nsk_options_count; i++) {
        if (nsk_options_table[i].option_short == option_short)
            return i;
    }

    nsk_err(
        "Error: getopt_long returned val out of bounds: %d\n",
        option_short
    );
    abort();
}

/*!
 * \brief  Error processing if no required argument is provided
 *
 * \param[in]  argv  The arguments array
 */
__attribute__((noreturn))
static void _arguments_noarg(char *const *argv) {
    if (optopt) {
        nsk_err(
            "Option -%c requires an argument\n",
            optopt
        );
    } else if (optind > 0 && argv[optind - 1]) {
        nsk_err(
            "Option --%s requires an argument\n",
            argv[optind - 1]
        );
    }
    exit(EXIT_FAILURE);
}

/*!
 * \brief  Error processing if unknown argument option is provided
 *
 * \param[in]  argv  The arguments array
 */
__attribute__((noreturn))
static void _arguments_unknown(char *const *argv) {
    if (optopt) {
        nsk_err(
            "Unknown option: -%c\n",
            optopt
        );
    } else if (optind > 0 && argv[optind - 1]) {
        nsk_err(
            "Unknown option: --%s\n",
            argv[optind - 1]
        );
    }
    exit(EXIT_FAILURE);
}

/*!
 * \brief  Processes the provided program arguments
 *
 * \param[in] argc  The count of arguments
 * \param[in] argv  The arguments array
 */
void nsk_args_process(int argc, char *const *argv) {
    _getopt_messagesdisable();

    nsk_auto_free char            *options_short = _options_short();
    nsk_auto_free struct option   *options_long  = _options_long();

    int c;
    int index;
    while (true) {
        c = getopt_long(
            argc,
            argv,
            options_short,
            options_long,
            &index
        );

        if (c == -1) {
            break;
        }

        switch (c) {
            case ':':
                _arguments_noarg(argv);

            case '?':
                _arguments_unknown(argv);

            default:
                nsk_options_table[_option_index(c)].option_processor();
                break;
        }
    }

    nsk_options_program.files = argv + optind;
}
