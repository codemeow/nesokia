#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "../../arguments/processors/nsk_option_ppuctrl34.h"
#include "../../arguments/nsk_args_options.h"
#include "../../utils/log/nsk_log_err.h"
#include "../../utils/nsk_util_size.h"

/*!
 * \brief  Sets the nametables purpose
 */
void nsk_option_ppuctrl34(void) {
    static const struct {
        const char               *value;
        enum nsk_ppuctlr34_values ppuctrl;
    } _table[] = {
        { "normal", NSK_PPUCTRL34_NORMAL },
        { "invert", NSK_PPUCTRL34_INVERT }
    };

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (strcmp(optarg, _table[i].value) == 0) {
            nsk_options_program.ppuctrl34 = _table[i].ppuctrl;
            return;
        }
    }

    nsk_err(
        "Invalid value \"%s\" for \"ppuctrl34\" option. "
            "Allowed values are \"normal\" and \"invert\"\n",
        optarg
    );
    exit(EXIT_FAILURE);
}