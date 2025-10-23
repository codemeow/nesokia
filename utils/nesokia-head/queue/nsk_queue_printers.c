#include <stdlib.h>
#include <string.h>

#include "../queue/nsk_queue_printers.h"

#include "../queue/printers/nsk_printer_json.h"
#include "../queue/printers/nsk_printer_md.h"
#include "../queue/printers/nsk_printer_table.h"
#include "../queue/printers/nsk_printer_tree.h"
#include "../queue/printers/nsk_printer_xml.h"
#include "../queue/printers/nsk_printer_yaml.h"
#include "../utils/log/nsk_log_err.h"
#include "../utils/nsk_util_size.h"

/*!
 * Printers table
 */
static const struct  {
    const char *name;                /*!< Name of the printer */
    nsk_printer_func_t printer;      /*!< Printer function    */
} nsk_printers_table[] = {
    { "tree",   nsk_printer_tree    },
    { "table",  nsk_printer_table   },
    { "json",   nsk_printer_json    },
    { "xml",    nsk_printer_xml     },
    { "yaml",   nsk_printer_yaml    },
    { "md",     nsk_printer_md      },
};

/*!
 * Size of the printers table
 */
static size_t nsk_printer_tablesize = NSK_SIZE(nsk_printers_table);

/*!
 * \brief  Checks if the printer name is valid
 *
 * \param[in] name  The name
 * \return True if the provided printer name is valid
 */
bool nsk_printer_isvalid(const char *name) {
    for (size_t i = 0; i < nsk_printer_tablesize; i++) {
        if (strcmp(nsk_printers_table[i].name, name) == 0) {
            return true;
        }
    }

    return false;
}

/*!
 * Returns the default printer if no printer is provided
 */
nsk_printer_func_t nsk_printer_default(void) {
    return nsk_printers_table[0].printer;
}

/*!
 * \brief  Returns function printer by name
 *
 * \param[in] name Printer name
 *
 * \return Printer function
 */
nsk_printer_func_t nsk_printer_func(const char *name) {
    for (size_t i = 0; i < nsk_printer_tablesize; i++) {
        if (strcmp(nsk_printers_table[i].name, name) == 0) {
            return nsk_printers_table[i].printer;
        }
    }

    nsk_err("Error: unknown printer name: \"%s\"\n", name);
    abort();
}