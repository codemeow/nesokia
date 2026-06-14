#include "nsk_process_input.h"
#include "../arguments/nsk_args_options.h"

#include "../formats/wav/nsk_wav_process.h"

/*!
 * \brief  Processes the input
 */
void nsk_process_input(void) {
    // Considering WAV for the first iteration
    nsk_wav_process(nsk_options_program.input.file);
}