#ifndef NSK_ARGS_PROCESS
#define NSK_ARGS_PROCESS

#include "../arguments/nsk_args_options.h"

/*!
 * \brief  Processes the provided program arguments
 *
 * \param[in] argc  The count of arguments
 * \param[in] argv  The arguments array
 * \return Argument processing result
 */
enum nsk_args_result nsk_args_process(int argc, char *const *argv);

#endif
