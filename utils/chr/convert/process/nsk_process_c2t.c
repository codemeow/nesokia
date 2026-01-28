#include <nsk_util_meta.h>

#include "../process/nsk_process_c2t.h"
#include "../arguments/nsk_args_options.h"

/*!
 * \brief  Validates inputcombinations
 */
static void _input_validate(void) {
    if (nsk_options_program.input.full) {
        nsk_err(
            "Invalid input: full input cannot be used in this mode\n"
        );
        exit(EXIT_FAILURE);
    }

    if (!nsk_options_program.input.ppucolors) {
        nsk_err(
            "Invalid input: PPU colors file must be provided\n"
        );
        exit(EXIT_FAILURE);
    }

    if (nsk_options_program.input.palettes.both &&
        (nsk_options_program.input.palettes.back ||
            nsk_options_program.input.palettes.sprites)
    ) {
        nsk_err(
            "Invalid input: Both united and separated palettes cannot be "
            "used at once\n"
        );
        exit(EXIT_FAILURE);
    }

    if (nsk_options_program.input.pattables.both &&
        (nsk_options_program.input.pattables.left ||
            nsk_options_program.input.pattables.right)
    ) {
        nsk_err(
            "Invalid input: Both united and separated pattern tables "
            "cannot be used at once\n"
        );
        exit(EXIT_FAILURE);
    }
}

/*!
 * \brief  Load the provided components
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
    *ppucolors = nsk_ppucolors_readpal(
        nsk_options_program.input.ppucolors
    );

    if (nsk_options_program.input.palettes.both) {
        *palettes = nsk_palettes_readspals(
            nsk_options_program.input.palettes.both
        );
    }

    if (nsk_options_program.input.palettes.back) {
        palettes->plane[NSK_PLANE_BACKGROUND] = nsk_palette_readspal(
            nsk_options_program.input.palettes.back
        );
    }

    if (nsk_options_program.input.palettes.sprites) {
        palettes->plane[NSK_PLANE_SPRITES] = nsk_palette_readspal(
            nsk_options_program.input.palettes.sprites
        );
    }

    if (nsk_options_program.input.pattables.both) {
        *pattables = nsk_pattables_readpats(
            nsk_options_program.input.pattables.both
        );
    }

    if (nsk_options_program.input.pattables.left) {
        pattables->plane[NSK_PLANE_BACKGROUND] = nsk_pattable_readpat(
            nsk_options_program.input.pattables.left
        );
    }

    if (nsk_options_program.input.pattables.right) {
        pattables->plane[NSK_PLANE_SPRITES] = nsk_pattable_readpat(
            nsk_options_program.input.pattables.right
        );
    }

    nsk_palettes_setcolors(ppucolors, palettes);

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

    nsk_pattables_settilespalettes(pattables, palettes);
    nsk_pattables_settilescolors  (pattables, palettes);
}

/*!
 * \brief  Validate provided PNG templates components
 *
 * \param[out]  ppucolors  The ppucolors
 * \param[out]  palettes   The palettes
 * \param[out]  pattables  The pattables
 */
static void _input_valdate(
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
    if (nsk_options_program.output.palettes.back ||
        nsk_options_program.output.palettes.sprites
    ) {
        nsk_err(
            "Output error: separate palette components cannot be used in "
            "this mode\n"
        );
        exit(EXIT_FAILURE);
    }

    if (nsk_options_program.output.pattables.both) {
        nsk_err(
            "Output error: united pattern table components cannot be "
            "used in this mode\n"
        );
        exit(EXIT_FAILURE);
    }
};

/*!
 * \brief  Saves PNG templates
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
    if (nsk_options_program.output.full) {
        nsk_auto_pifree struct nsk_type_pngimage *image = nsk_pngimage_composesave(
            ppucolors,
            palettes,
            pattables
        );
        nsk_pngimage_write(
            image,
            nsk_options_program.output.full
        );
    }

    if (nsk_options_program.output.ppucolors) {
        nsk_auto_pifree struct nsk_type_pngimage *image = nsk_ppucolors_convtopng(
            ppucolors
        );
        nsk_pngimage_write(
            image,
            nsk_options_program.output.ppucolors
        );
    }

    if (nsk_options_program.output.palettes.both) {
        nsk_auto_pifree struct nsk_type_pngimage *image = nsk_palettes_convtopng(
            palettes
        );
        nsk_pngimage_write(
            image,
            nsk_options_program.output.palettes.both
        );
    }

    if (nsk_options_program.output.pattables.left) {
        nsk_auto_pifree struct nsk_type_pngimage *image = nsk_pattable_convtopng(
            nsk_pattables_getbyaddress(
                pattables,
                NSK_PATTABLEADDR_0000
            )
        );
        nsk_pngimage_write(
            image,
            nsk_options_program.output.pattables.left
        );
    }

    if (nsk_options_program.output.pattables.right) {
        nsk_auto_pifree struct nsk_type_pngimage *image = nsk_pattable_convtopng(
            nsk_pattables_getbyaddress(
                pattables,
                NSK_PATTABLEADDR_1000
            )
        );
        nsk_pngimage_write(
            image,
            nsk_options_program.output.pattables.right
        );
    }
}

/*!
 * \brief  Processes c2t mode
 */
void nsk_process_c2t(void) {
    _input_validate();

    struct nsk_type_ppucolors ppucolors = { 0 };
    struct nsk_type_palettes  palettes  = { 0 };
    struct nsk_type_pattables pattables = { 0 };

    _input_load(
        &ppucolors,
        &palettes,
        &pattables
    );

    _input_valdate(
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