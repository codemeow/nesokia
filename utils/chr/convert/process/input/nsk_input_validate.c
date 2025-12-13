#include <stdlib.h>

#include "../../utils/log/nsk_log_err.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Validates all input files
 */
void nsk_input_validate(void) {
    /* Non-composite mode */
    if (nsk_options_program.files[0]) {

        if (nsk_options_program.files[1]) {
            nsk_err(
                "Only one input image is allowed\n"
            );
            exit(EXIT_FAILURE);
        }

        if (nsk_options_program.input.template ||
            nsk_options_program.input.left     ||
            nsk_options_program.input.right    ||
            nsk_options_program.input.colors   ||
            nsk_options_program.input.palettes) {

            nsk_err(
                "Simple input and composite input cannot be mixed\n"
            );
            exit(EXIT_FAILURE);
        }

    /* Composite mode */
    } else {

        if (!nsk_options_program.input.template) {

            if (!(nsk_options_program.input.left   &&
                  nsk_options_program.input.right  &&
                  nsk_options_program.input.colors &&
                  nsk_options_program.input.palettes)
                ) {

                nsk_err(
                    "Template image must be passed if one or more composite "
                        "components are missing\n"
                );
                exit(EXIT_FAILURE);
            }
        }
    }
}