#include "../process/nsk_process_vars.h"

/*!
 * \brief  Global instance of the compiled input image
 */
struct nsk_image *nsk_image_input = NULL;

/*!
 * \brief  Module deinitializer
 */
__attribute__((destructor))
static void _fini(void) {
    nsk_image_free(nsk_image_input);
}