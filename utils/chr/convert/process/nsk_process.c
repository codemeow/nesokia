#include <assert.h>

#include <nsk_util_meta.h>

#include "../process/nsk_process.h"
#include "../arguments/nsk_args_options.h"

#include "../process/nsk_process_c2t.h"
#include "../process/nsk_process_t2c.h"

/*!
 * \brief Mode processor callback type
 */
typedef void (*nsk_mode_processor_t)(void);

/*!
 * \brief  Selects the mode to process files
 */
void nsk_process(void) {
    static const nsk_mode_processor_t _table[] = {
        [NSK_MODE_CHR2TEMPLATE] = nsk_process_c2t,
        [NSK_MODE_TEMPLATE2CHR] = nsk_process_t2c
    };

    static_assert(
        NSK_SIZE(_table) == NSK_MODES_COUNT,
        "Invalid number of elements in `nsk_process_input`"
    );

    _table[nsk_options_program.mode]();
}