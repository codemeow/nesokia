#ifndef NSK_UTIL_CLEANUP
#define NSK_UTIL_CLEANUP

#include <stdio.h>

/*!
 * Automatically free the allocated memory on exit
 */
#define nsk_auto_free __attribute__((cleanup(_nsk_auto_free)))

/*!
 * \brief  Autocleaner for memory pointers
 *
 * \param[in, out]  ptr  The pointer
 */
void _nsk_auto_free(void *ptr);

/*!
 * Automatically close the opened file on exit
 */
#define nsk_auto_fclose __attribute__((cleanup(_nsk_auto_fclose)))

/*!
 * \brief  Autocleaner for opened files
 *
 * \param[in, out]  file  The file
 */
void _nsk_auto_fclose(FILE **file);

#endif