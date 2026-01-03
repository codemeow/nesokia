#include <getopt.h>
#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_output.h"
#include "../../arguments/nsk_args_options.h"
#include "../../queue/nsk_queue_printers.h"

/*!
 * \brief  Selects the output type (tree, table, etc)
 */
void nsk_option_output(void) {
    if (!nsk_printer_isvalid(optarg)) {
        nsk_err("Error: invalid value \"%s\" for \"printer\" option\n", optarg);
        exit(EXIT_FAILURE);
    }

    nsk_options_program.printer = optarg;
}