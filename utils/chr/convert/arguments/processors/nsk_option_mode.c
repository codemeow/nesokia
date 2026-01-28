#include <getopt.h>
#include <string.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_mode.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Selects conversion mode
 */
void nsk_option_mode(void) {
    static const struct {
        enum nsk_mode_values mode;
        const char *string;
    } _table[] = {
        { NSK_MODE_TEMPLATE2CHR, "t2c" },
        { NSK_MODE_CHR2TEMPLATE, "c2t" },
    };

    static_assert(
        NSK_SIZE(_table) == NSK_MODES_COUNT,
        "Invalid number of elements in `nsk_option_mode`"
    );

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (strcmp(optarg, _table[i].string) == 0) {
            nsk_options_program.mode = _table[i].mode;
            return;
        }
    }

    nsk_err(
        "Unknown mode option: %s\n"
        "See '--help' for the list of options\n",
        optarg
    );
    exit(EXIT_FAILURE);
}
