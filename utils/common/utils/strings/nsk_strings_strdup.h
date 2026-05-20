#ifndef NSK_STRINGS_STRDUP
#define NSK_STRINGS_STRDUP

/*!
 * \brief  Dupes the string value
 *
 * \param[in] value  The value
 * \return Allocated copied string
 *
 * \note This function never returns NULL. It either returns an allocated copy
 *       or terminates the process.
 * \note Passing NULL terminates the process.
 */
char *nsk_strings_strdup(const char *value);

#endif
