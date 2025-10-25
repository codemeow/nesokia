#include "../../arguments/processors/nsk_option_follow.h"

#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Sets the follow symbolic links flag
 */
void nsk_option_follow(void) {
    nsk_options_program.follow = true;
}