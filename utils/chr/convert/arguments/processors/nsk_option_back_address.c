#include <getopt.h>
#include <string.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_back_address.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets background pattern table address
 */
void nsk_option_back_address(void) {
    static const struct {
        enum nsk_ppuctlr34_values value;
        const char *string;
    } _table[] = {
        { NSK_PPUCTRL34_BACK0000, "0000" },
        { NSK_PPUCTRL34_BACK1000, "1000" },
    };

    static_assert(
        NSK_SIZE(_table) == NSK_PPUCTRL34S_COUNT,
        "Invalid number of elements in `nsk_option_back_address`"
    );

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (strcmp(optarg, _table[i].string) == 0) {
            nsk_options_program.ppuctrl34 = _table[i].value;
            return;
        }
    }

    nsk_err(
        "Unknown back-address option: %s\n"
        "See '--help' for the list of options\n",
        optarg
    );
    exit(EXIT_FAILURE);
}
