#include <nsk_util_meta.h>

#include "../process/nsk_process_input.h"
#include "../types/nsk_type_colors.h"
#include "../types/nsk_type_image.h"
#include "../types/nsk_type_nametable.h"
#include "../types/nsk_type_palette.h"

/*!
 * \brief  Processes and merges input files
 */
void nsk_process_input(void) {
    static const struct {
        void (*reader   )(void);
        void (*validator)(void);
    } _table[] = {
        { nsk_image_read,       nsk_image_validate      },
        { nsk_colors_read,      nsk_colors_validate     },
        { nsk_palettes_read,    nsk_palettes_validate   },
        { nsk_nametables_read,  nsk_nametables_validate }
    };

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        _table[i].reader();
        _table[i].validator();
    }
}