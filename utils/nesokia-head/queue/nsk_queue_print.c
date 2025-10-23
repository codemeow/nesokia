#include "../queue/nsk_queue_print.h"

#include "../arguments/nsk_args_options.h"
#include "../queue/nsk_queue_printers.h"

/*!
 * \brief  Show the records, that were pushed to the queue
 */
void nsk_queue_print(void) {
    if (nsk_options_program.printer) {
        nsk_printer_func(nsk_options_program.printer)();
    } else {
        nsk_printer_default()();
    }
}