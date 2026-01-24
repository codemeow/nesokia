#include <threads.h>
#include <nsk_util_meta.h>

#include "../process/nsk_process_output.h"
#include "../arguments/nsk_args_options.h"
#include "../types/nsk_type_colors.h"
#include "../types/nsk_type_palette.h"

/*!
 * \brief  Processes output files
 */
void nsk_process_output(void) {
    if (nsk_options_program.output.ansi) {
        nsk_colors_ansi();
        nsk_palettes_ansi();
    }

    if (nsk_options_program.output.colors) {
        nsk_colors_save();
    }

    if (nsk_options_program.output.palettes) {
        nsk_palettes_save();
    }
}
