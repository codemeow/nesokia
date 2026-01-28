#ifndef NSK_PROCESS_VARS
#define NSK_PROCESS_VARS

#include <nsk_util_meta.h>

/*!
 * \brief  Input data type
 */
struct nsk_type_input {
    union {
        struct {
            struct nsk_type_pngimage *template;  /*!< Full PNG template       */
        } t2c;
        struct {
            struct nsk_type_ppucolors ppucolors; /*!< CHR PPU colors          */
            struct nsk_type_palettes  palettes;  /*!< CHR local palettes      */
            struct nsk_type_pattables tables;    /*!< CHR pattern tables      */
        } c2t;
    };
};

/*!
 * \brief  Global input data
 */
extern struct nsk_type_input nsk_input;

#endif