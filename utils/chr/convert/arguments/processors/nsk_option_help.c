#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_help.h"
#include "../../arguments/nsk_args_options.h"

#if !defined(NSK_PROGRAM_NAME)
    #error "NSK_PROGRAM_NAME should be defined with the program name"
#endif

/*!
 * \brief  Displays program help and exits
 */
enum nsk_args_result nsk_option_help(void) {
    nsk_inf("Usage: %s [options]...\n", NSK_PROGRAM_NAME);
    nsk_inf("Convert between PNG templates and NES CHR-related data.\n");
    nsk_inf("\n");
    for (size_t i = 0; i < nsk_options_count; i++) {
        nsk_inf("    ");
        if (nsk_options_table[i].option_short < NSK_OPTION_SHORTLIMIT) {
            nsk_inf("-%c", nsk_options_table[i].option_short);
        } else {
            nsk_inf("  ");
        }

        if (nsk_options_table[i].option_long) {
            if (nsk_options_table[i].option_short < NSK_OPTION_SHORTLIMIT) {
                nsk_inf(", ");
            }

            nsk_inf("--%s", nsk_options_table[i].option_long);
            switch (nsk_options_table[i].option_arg) {
                case optional_argument:
                    nsk_inf("[=value]");
                    break;

                case required_argument:
                    nsk_inf("=value");
                    break;
            }
            nsk_inf("\n");
        }

        nsk_inf("%s", nsk_options_table[i].option_desc);
        nsk_inf("\n");
    }

    return NSK_ARGS_EXIT_SUCCESS;
}
