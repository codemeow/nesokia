#include <stdbool.h>
#include <nsk_util_meta.h>

#include "../process/nsk_process_input.h"
#include "../arguments/nsk_args_options.h"
#include "../types/nsk_type_colors.h"
#include "../types/nsk_type_palette.h"

/*!
 * \brief  List of the input/output flags for validation
 */
enum nsk_options_flags {
    NSK_OPTFLAG_INPUT_COLORS     = 1 << 0, /*!< input.colors        */
    NSK_OPTFLAG_INPUT_PALBACK    = 1 << 1, /*!< input.palback       */
    NSK_OPTFLAG_INPUT_PALSPRITES = 1 << 2, /*!< input.palsprites    */
    NSK_OPTFLAG_INPUT_PALMERGED  = 1 << 3, /*!< input.palmerged     */
    NSK_OPTFLAG_OUTPUT_ANSI      = 1 << 4, /*!< output.ansi         */
    NSK_OPTFLAG_OUTPUT_COLORS    = 1 << 5, /*!< output.colors       */
    NSK_OPTFLAG_OUTPUT_PALETTES  = 1 << 6, /*!< output.palettes     */
};

/*!
 * \brief  Makes a bitlist of the present options
 *
 * \return bit flags
 */
static unsigned _options_present(void) {
    static const struct {
        const void            *target;
        bool                   ischar;
        enum nsk_options_flags flag;
    } _table[] = {
        { &nsk_options_program.input.colors,     true,  NSK_OPTFLAG_INPUT_COLORS        },
        { &nsk_options_program.input.palback,    true,  NSK_OPTFLAG_INPUT_PALBACK       },
        { &nsk_options_program.input.palsprites, true,  NSK_OPTFLAG_INPUT_PALSPRITES    },
        { &nsk_options_program.input.palmerged,  true,  NSK_OPTFLAG_INPUT_PALMERGED     },
        { &nsk_options_program.output.ansi,      false, NSK_OPTFLAG_OUTPUT_ANSI         },
        { &nsk_options_program.output.colors,    true,  NSK_OPTFLAG_OUTPUT_COLORS       },
        { &nsk_options_program.output.palettes,  true,  NSK_OPTFLAG_OUTPUT_PALETTES     },
    };

    unsigned result = 0;
    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if (_table[i].ischar) {
            if (*(const char **)_table[i].target) {
                result |= _table[i].flag;
            }
        } else {
            if (*(const bool *)_table[i].target) {
                result |= _table[i].flag;
            }
        }
    }

    return result;
}

/*!
 * \brief  Validates the combination of the input options
 */
void _input_options_validate(void) {
    static const struct {
        unsigned    present_mask;
        unsigned    missing_mask;
        const char *message;
    } _table[] = {
        {
            .present_mask = 0,
            .missing_mask =
                NSK_OPTFLAG_INPUT_COLORS     |
                NSK_OPTFLAG_INPUT_PALBACK    |
                NSK_OPTFLAG_INPUT_PALSPRITES |
                NSK_OPTFLAG_INPUT_PALMERGED,
            .message = "no input options specified"
        },
        {
            .present_mask = 0,
            .missing_mask =
                NSK_OPTFLAG_INPUT_COLORS,
            .message = "no input PPU colors specified"
        },
        {
            .present_mask =
                NSK_OPTFLAG_INPUT_PALBACK    |
                NSK_OPTFLAG_INPUT_PALSPRITES |
                NSK_OPTFLAG_INPUT_PALMERGED,
            .missing_mask = 0,
            .message = "separate and merged palettes cannot be processed at once"
        },
        {
            .present_mask =
                NSK_OPTFLAG_INPUT_PALBACK,
            .missing_mask =
                NSK_OPTFLAG_INPUT_PALSPRITES,
            .message = "sprites palette was not specified"
        },
        {
            .present_mask =
                NSK_OPTFLAG_INPUT_PALSPRITES,
            .missing_mask =
                NSK_OPTFLAG_INPUT_PALBACK,
            .message = "background palette was not specified"
        },
        {
            .present_mask = 0,
            .missing_mask =
                NSK_OPTFLAG_OUTPUT_ANSI     |
                NSK_OPTFLAG_OUTPUT_COLORS   |
                NSK_OPTFLAG_OUTPUT_PALETTES,
            .message = "no output specified"
        },
        {
            .present_mask =
                NSK_OPTFLAG_OUTPUT_PALETTES,
            .missing_mask =
                NSK_OPTFLAG_INPUT_PALBACK    |
                NSK_OPTFLAG_INPUT_PALSPRITES |
                NSK_OPTFLAG_INPUT_PALMERGED,
            .message = "no palettes specified"
        }
    };

    unsigned present = _options_present();

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        if ((present & _table[i].present_mask) == _table[i].present_mask &&
            (present & _table[i].missing_mask) == 0
        ) {
            nsk_err("Error: %s\n", _table[i].message);
            exit(EXIT_FAILURE);
        }
    }
}

/*!
 * \brief  Processes input files
 */
void nsk_process_input(void) {

    static const struct {
        void (*reader   )(void);
        void (*validator)(void);
    } _table[] = {
        { nsk_colors_read,      nsk_colors_validate     },
        { nsk_palettes_read,    nsk_palettes_validate   }
    };

    _input_options_validate();

    for (size_t i = 0; i < NSK_SIZE(_table); i++) {
        _table[i].reader();
        _table[i].validator();
    }
}
