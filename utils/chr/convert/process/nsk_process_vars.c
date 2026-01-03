#include "../process/nsk_process_vars.h"

/*!
 * \brief  Global input data
 */
struct nsk_type_input nsk_input = { 0 };

/*!
 * \brief  Module deinitializer
 */
__attribute__((destructor))
static void _fini(void) {
    nsk_image_free(nsk_input.image);
}