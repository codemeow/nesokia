#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output.h"
#include "../../arguments/nsk_args_options.h"
#include "../../queue/nsk_queue_printers.h"

/*!
 * \brief  Selects the output type (tree, table, etc)
 */
enum nsk_args_result nsk_option_output(void) {
    if (!nsk_printer_isvalid(optarg)) {
        nsk_err("Error: invalid value \"%s\" for \"printer\" option\n", optarg);
        return NSK_ARGS_EXIT_FAILURE;
    }

    nsk_options_program.printer = optarg;

    return NSK_ARGS_CONTINUE;
}
