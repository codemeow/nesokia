#include <string.h>

#include <nsk_util_meta.h>

#include "../../queue/printers/nsk_printer_match.h"
#include "../../arguments/nsk_args_options.h"

/*!
 * \brief  Checks if the field should be printed
 *
 * \param[in] field  The field
 * \return True if the field should be printed (either because this field is
 * directly asked or no fields filter set at all)
 */
bool nsk_printer_match(const char *field) {
    if (!nsk_options_program.keys) {
        /* No filter is set - print any field */
        return true;
    }

    struct nsk_pair *pair = nsk_options_program.keys;
    while (pair) {
        if (strcmp(pair->name, field) == 0) {
            return true;
        }

        pair = nsk_pair_next(pair);
    }

    return false;
}