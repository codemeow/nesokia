#ifndef NSK_UTIL_CLEANUP
#define NSK_UTIL_CLEANUP

#include <stdio.h>

/*!
 * Automatically free the allocated memory on block exit
 */
#define nsk_auto_free __attribute__((cleanup(_nsk_auto_free)))

/*!
 * \brief  Autocleaner for memory pointers
 *
 * \param[in, out]  ptr  The pointer
 */
void _nsk_auto_free(void *ptr);

/*!
 * Automatically close the opened file on block exit
 */
#define nsk_auto_fclose __attribute__((cleanup(_nsk_auto_fclose)))

/*!
 * \brief  Autocleaner for opened files
 *
 * \param[in, out]  file  The file
 */
void _nsk_auto_fclose(FILE **file);

/*!
 * \brief  "Steals" the pointer from the cleanup function,
 * allowing to return the value in successfull branches:
 * ~~~
 * nsk_auto_type type var = ...
 *
 * if (!check)
 *     return false; // safe, var will be cleaned
 *
 * return nsk_steal(var); // also safe, stealer will cancel cleaning
 * ~~~
 *
 * \param[in]  ptr  The pointer
 * \return Same data via another pointer
 */
#define nsk_steal(ptr) \
    ({ __auto_type _ret = (ptr); (ptr) = NULL; _ret; })

#endif