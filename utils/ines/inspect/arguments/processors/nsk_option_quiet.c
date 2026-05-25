#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_quiet.h"
/*!
 * \brief  Sets the "quiet" mode, producing no output but errors
 */
enum nsk_args_result nsk_option_quiet(void) {
    nsk_log_quiet = true;

    return NSK_ARGS_CONTINUE;
}
