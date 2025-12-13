#include "../process/nsk_process_input.h"

#include "../process/input/nsk_input_validate.h"
#include "../process/input/nsk_input_read.h"

/*!
 * \brief  Processes and merges input files
 */
void nsk_process_input(void) {
    nsk_input_validate();
    nsk_input_read();
}