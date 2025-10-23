#ifndef NSK_QUEUE_PRINTERS
#define NSK_QUEUE_PRINTERS

#include <stdbool.h>
#include <stddef.h>

/*!
 * Printer function type;
 */
typedef void (*nsk_printer_func_t)(void);

/*!
 * \brief  Checks if the printer name is valid
 *
 * \param[in] name  The name
 * \return True if the provided printer name is valid
 */
bool nsk_printer_isvalid(const char *name);

/*!
 * Returns the default printer if no printer is provided
 *
 * \return Printer function
 */
nsk_printer_func_t nsk_printer_default(void);

/*!
 * \brief  Returns function printer by name
 *
 * \param[in] name Printer name
 *
 * \return Printer function
 */
nsk_printer_func_t nsk_printer_func(const char *name);

#endif