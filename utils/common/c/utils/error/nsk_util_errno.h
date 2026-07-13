#ifndef NSK_UTIL_ERRNO
#define NSK_UTIL_ERRNO

/*!
 * \brief  Returns the text message related to the provided code
 *
 * \param[in] errnum  The errno code
 * \return Static string
 */
const char *nsk_util_strerror(unsigned errnum);

#endif