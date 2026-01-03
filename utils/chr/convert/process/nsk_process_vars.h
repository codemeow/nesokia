#ifndef NSK_PROCESS_VARS
#define NSK_PROCESS_VARS

#include "../types/nsk_type_image.h"
#include "../types/nsk_type_colors.h"
#include "../types/nsk_type_palette.h"
#include "../types/nsk_type_nametable.h"

/*!
 * \brief  Input data type
 */
struct nsk_type_input {
    struct nsk_type_image       *image;      /*!< Input image           */
    struct nsk_type_colors       colors;     /*!< INput global palette  */
    struct nsk_type_palettes     palettes;   /*!< Input local palettes  */
    struct nsk_type_nametables   nametables; /*!< Input nametables      */
};

/*!
 * \brief  Global input data
 */
extern struct nsk_type_input nsk_input;

#endif