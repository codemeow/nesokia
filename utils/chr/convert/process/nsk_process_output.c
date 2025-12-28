#include <stdbool.h>
#include <stddef.h>

#include "../process/nsk_process_output.h"

#include "../arguments/nsk_args_options.h"
#include "../process/output/nsk_output_palette.h"
#include "../process/output/nsk_output_nametable.h"
#include "../utils/nsk_util_size.h"

/*!
 * \brief  Sets the default names if no names provided
 */
static void _output_setdefaults(void) {
    static const char default_left[]    = "0000.chr";
    static const char default_right[]   = "1000.chr";
    static const char default_back[]    = "back.pal";
    static const char default_sprites[] = "sprites.pal";

    if (!nsk_options_program.output.chr0000 &&
        !nsk_options_program.output.chr1000 &&
        !nsk_options_program.output.palback &&
        !nsk_options_program.output.palsprites) {

        nsk_options_program.output.chr0000      = default_left;
        nsk_options_program.output.chr1000      = default_right;
        nsk_options_program.output.palback      = default_back;
        nsk_options_program.output.palsprites   = default_sprites;
    }
}

/*!
 * \brief  Processes and saves output files
 */
void nsk_process_output(void) {
    _output_setdefaults();

    // const char *paldir =
    //     nsk_options_program.directory.palettes ?
    //     nsk_options_program.directory.palettes :
    //     nsk_options_program.directory.output;

    // const struct { /*!< Check if this component is selected  */
    //     const char *dir;  /*!< User selected savedir                */
    //     const char *name; /*!< User selected savename               */
    //     size_t x;         /*!< Component X position                 */
    //     size_t y;         /*!< Component Y position                 */
    //     void (*convert)(  /*!< Conversion function                  */
    //         size_t x,
    //         size_t y,
    //         const char *dir,
    //         const char *name
    //     );
    // } _table[] = {
    //     {
    //         .dir      = paldir,
    //         .name     = nsk_options_program.output.palback,
    //         .x        = NSK_POS_PALETTEB_X,
    //         .y        = NSK_POS_PALETTEB_Y,
    //         .convert  = nsk_output_palette
    //     },
    //     {
    //         .dir      = paldir,
    //         .name     = nsk_options_program.output.palsprites,
    //         .x        = NSK_POS_PALETTES_X,
    //         .y        = NSK_POS_PALETTES_Y,
    //         .convert  = nsk_output_palette
    //     },
    //     {
    //         .dir      = nsk_options_program.directory.output,
    //         .name     = nsk_options_program.output.chr0000,
    //         .x        = NSK_POS_LEFT_X,
    //         .y        = NSK_POS_LEFT_Y,
    //         .convert  = nsk_output_nametable
    //     },
    //     {
    //         .dir      = nsk_options_program.directory.output,
    //         .name     = nsk_options_program.output.chr1000,
    //         .x        = NSK_POS_RIGHT_X,
    //         .y        = NSK_POS_RIGHT_Y,
    //         .convert  = nsk_output_nametable
    //     }
    // };

    // for (size_t i = 0; i < NSK_SIZE(_table); i++) {
    //     if (!_table[i].name) {
    //         continue;
    //     }

    //     _table[i].convert (
    //         _table[i].x,
    //         _table[i].y,
    //         _table[i].dir,
    //         _table[i].name
    //     );
    // }

}