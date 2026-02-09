#include <nsk_util_meta.h>

#include "../process/nsk_process_t2c.h"
#include "../arguments/nsk_args_options.h"

/*!
 * \brief  Validate input combinations
 */
static void _options_validate(void) {
    if (nsk_options_program.input.full) {
        if (nsk_options_program.input.ppucolors         ||
            nsk_options_program.input.palettes.both     ||
            nsk_options_program.input.palettes.back     ||
            nsk_options_program.input.palettes.sprites  ||
            nsk_options_program.input.pattables.both    ||
            nsk_options_program.input.pattables.left    ||
            nsk_options_program.input.pattables.right
        ) {
            nsk_err(
                "Invalid input: no other input options should be used if "
                "full PNG template is provided\n"
            );
            exit(EXIT_FAILURE);
        }

    } else {
        if (!nsk_options_program.input.ppucolors) {
            nsk_err(
                "Invalid input: no PPU colors PNG template is provided\n"
            );
            exit(EXIT_FAILURE);
        }

        if (!nsk_options_program.input.palettes.both) {
            nsk_err(
                "Invalid input: no palettes PNG template is provided\n"
            );
            exit(EXIT_FAILURE);
        }

        if (nsk_options_program.input.palettes.back ||
            nsk_options_program.input.palettes.sprites
        ) {
            nsk_err(
                "Invalid input: separate palettes input cannot be used in "
                "this mode\n"
            );
            exit(EXIT_FAILURE);
        }

        if (nsk_options_program.input.pattables.both) {
            nsk_err(
                "Invalid input: united pattern tables input cannot be "
                "used in this mode\n"
            );
            exit(EXIT_FAILURE);
        }

        if (!nsk_options_program.input.pattables.left
        ) {
            nsk_err(
                "Invalid input: no left pattern table provided\n"
            );
            exit(EXIT_FAILURE);
        }

        if (!nsk_options_program.input.pattables.right
        ) {
            nsk_err(
                "Invalid input: no right pattern table provided\n"
            );
            exit(EXIT_FAILURE);
        }
    }
}

/*!
 * \brief  Load the provided PNG templates
 *
 * \param[out]  ppucolors  The ppucolors
 * \param[out]  palettes   The palettes
 * \param[out]  pattables  The pattables
 */
static void _input_load(
    struct nsk_type_ppucolors *ppucolors,
    struct nsk_type_palettes  *palettes,
    struct nsk_type_pattables *pattables
) {
    if (nsk_options_program.input.full) {
        nsk_pngimage_composeread(
            nsk_options_program.input.full,
            ppucolors,
            palettes,
            pattables
        );

    } else {
        *ppucolors = nsk_ppucolors_readpng(
            nsk_options_program.input.ppucolors
        );
        *palettes = nsk_palettes_readpng(
            nsk_options_program.input.palettes.both
        );
        pattables->plane[NSK_PLANE_BACKGROUND] = nsk_pattable_readpng(
            nsk_options_program.input.pattables.left
        );
        pattables->plane[NSK_PLANE_SPRITES] = nsk_pattable_readpng(
            nsk_options_program.input.pattables.left
        );
    }

    nsk_pattable_setplane(
        &pattables->plane[NSK_PLANE_BACKGROUND],
        NSK_PLANE_BACKGROUND
    );
    nsk_pattable_setplane(
        &pattables->plane[NSK_PLANE_SPRITES],
        NSK_PLANE_SPRITES
    );
    nsk_pattable_setaddress(
        &pattables->plane[NSK_PLANE_BACKGROUND],
        NSK_PATTABLEADDR_0000
    );
    nsk_pattable_setaddress(
        &pattables->plane[NSK_PLANE_SPRITES],
        NSK_PATTABLEADDR_1000
    );

    if (nsk_options_program.ppuctrl34 == NSK_PPUCTRL34_BACK1000) {
        nsk_pattable_swapaddress(pattables);
    }

    nsk_palettes_setindexes(
        ppucolors,
        palettes
    );
    nsk_pattables_settilespalettes(
        pattables,
        palettes,
        nsk_options_program.input.explicit
    );
    nsk_pattables_settilesindexes (pattables, palettes);
}

/*!
 * \brief  Validate provided PNG templates components
 *
 * \param[out]  ppucolors  The ppucolors
 * \param[out]  palettes   The palettes
 * \param[out]  pattables  The pattables
 */
static void _input_validate(
    struct nsk_type_ppucolors *ppucolors,
    struct nsk_type_palettes  *palettes,
    struct nsk_type_pattables *pattables __attribute__((unused))
) {
    nsk_ppucolors_validate(ppucolors);
    nsk_palettes_validate(ppucolors, palettes);
}

/*!
 * \brief  Shows provided PNG templates components
 *
 * \param[out]  ppucolors  The ppucolors
 * \param[out]  palettes   The palettes
 * \param[out]  pattables  The pattables
 */
static void _input_show(
    struct nsk_type_ppucolors *ppucolors,
    struct nsk_type_palettes  *palettes,
    struct nsk_type_pattables *pattables
) {
    nsk_ppucolors_show(ppucolors);
    nsk_palettes_show (palettes);
    nsk_pattables_show(pattables);
}

/*!
 * \brief  Validate output combinations
 */
static void _output_validate(void) {
    if (nsk_options_program.output.full) {
        nsk_err(
            "Invalid output: full template cannot be used in this mode\n"
        );
        exit(EXIT_FAILURE);
    }
}

/*!
 * \brief  Saves CHR files
 *
 * \param[out]  ppucolors  The ppucolors
 * \param[out]  palettes   The palettes
 * \param[out]  pattables  The pattables
 */
static void _output_save(
    struct nsk_type_ppucolors *ppucolors,
    struct nsk_type_palettes  *palettes,
    struct nsk_type_pattables *pattables
) {
    if (nsk_options_program.output.ppucolors) {
        nsk_ppucolors_savepal(
            nsk_options_program.output.ppucolors,
            ppucolors
        );
    }

    if (nsk_options_program.output.palettes.both) {
        nsk_palettes_savespals(
            nsk_options_program.output.palettes.both,
            palettes
        );
    }

    if (nsk_options_program.output.palettes.back) {
        nsk_palette_savespal(
            nsk_options_program.output.palettes.back,
            &palettes->plane[NSK_PLANE_BACKGROUND]
        );
    }

    if (nsk_options_program.output.palettes.sprites) {
        nsk_palette_savespal(
            nsk_options_program.output.palettes.sprites,
            &palettes->plane[NSK_PLANE_SPRITES]
        );
    }

    if (nsk_options_program.output.pattables.both) {
        nsk_pattables_savepats(
            nsk_options_program.output.pattables.both,
            pattables
        );
    }

    if (nsk_options_program.output.pattables.left) {
        nsk_pattable_savepat(
            nsk_options_program.output.pattables.left,
            nsk_pattables_getbyaddress(
                pattables,
                NSK_PATTABLEADDR_0000
            )
        );
    }

    if (nsk_options_program.output.pattables.right) {
        nsk_pattable_savepat(
            nsk_options_program.output.pattables.right,
            nsk_pattables_getbyaddress(
                pattables,
                NSK_PATTABLEADDR_1000
            )
        );
    }
}

/*!
 * \brief  Processes t2c mode
 */
void nsk_process_t2c(void) {
    _options_validate();

    struct nsk_type_ppucolors ppucolors = { 0 };
    struct nsk_type_palettes  palettes  = { 0 };
    struct nsk_type_pattables pattables = { 0 };

    _input_load(
        &ppucolors,
        &palettes,
        &pattables
    );

    _input_validate(
        &ppucolors,
        &palettes,
        &pattables
    );

    _input_show(
        &ppucolors,
        &palettes,
        &pattables
    );

    _output_validate();

    _output_save(
        &ppucolors,
        &palettes,
        &pattables
    );
}