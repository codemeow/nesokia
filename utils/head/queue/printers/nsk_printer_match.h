#ifndef NSK_PRINTER_MATCH
#define NSK_PRINTER_MATCH

#include <stdbool.h>

/*!
 * \brief  Checks if the field should be printed
 *
 * \param[in] field  The field
 * \return True if the field should be printed (either because this field is
 * directly asked or no fields filter set at all)
 */
bool nsk_printer_match(const char *field);

#endif