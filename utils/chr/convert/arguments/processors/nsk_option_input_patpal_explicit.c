#include <getopt.h>
#include <stdio.h>

#include <nsk_util_meta.h>

#include "../../arguments/processors/nsk_option_input_patpal_explicit.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Reads the list of explicitly requested tile palettes
 */
static void _explicit_read(void) {
    const char partnames[]  = "lr";
    const size_t partsize   = 256;
    const size_t partscount = 2;
    const size_t list_size  = partsize * partscount;

    char **list = malloc(sizeof(*list) * list_size);
    for (size_t p = 0; p < partscount; p++) {
        for (size_t i = 0; i < partsize; i++) {
            int res = asprintf(
                &list[p * partsize + i],
                "%c%02zx",
                partnames[p],
                i
            );
            if (res == -1) {
                nsk_err(
                    "Cannot allocate memory for explicit palette list"
                );
                exit(EXIT_FAILURE);
            }
        }
    }

    nsk_options_program.input.explicit = nsk_pair_parse(
        optarg,
        NSK_PAIR_NAME | NSK_PAIR_OPERATOR | NSK_PAIR_VALUE,
        (const char *const *)list
    );

    for (size_t i = 0; i < list_size; i++) {
        free(list[i]);
    }

    free(list);
}

/*!
 * \brief  Validates the list
 */
static void _explicit_validate(void) {
    struct nsk_pair *pair = nsk_options_program.input.explicit;
    while (pair) {
        if (pair->operator != NSK_PAIR_EQUAL) {
            nsk_err(
                "The explicit palettes list cannot contain any operator but '='\n"
                "The provided list is as follows:\"%s\"\n",
                optarg
            );
            exit(EXIT_FAILURE);
        }

        pair = nsk_pair_next(pair);
    }
}

/*!
 * \brief  Sets input list of explicit palette selection
 */
void nsk_option_input_patpal_explicit(void) {
    _explicit_read();
    _explicit_validate();
}