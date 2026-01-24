#ifndef NSK_PROCESS_VARS
#define NSK_PROCESS_VARS

#include <nsk_util_meta.h>

struct nsk_type_input {
    struct nsk_type_ppucolors colors;   /*!< Input global palette (PPU colors) */
    struct nsk_type_palettes  palettes; /*!< Input local palettes              */

    struct {
        struct nsk_type_image *colors;   /*!< Colors empty template   */
        struct nsk_type_image *palettes; /*!< Palettes empty template */
    } templates;
};

/*!
 * \brief  Global input data
 */
extern struct nsk_type_input nsk_input;

#endif