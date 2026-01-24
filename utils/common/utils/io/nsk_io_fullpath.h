#ifndef NSK_IO_FULLPATH
#define NSK_IO_FULLPATH

/*!
 * \brief Program name passed to as argv[0]
 */
extern const char *nsk_args_program;

/*!
 * \brief  Resolves relative path to the full path starting with the current
 * binary position
 *
 * \param[in] path  The relative path
 * \return Static string with full resolved path
 */
const char *nsk_io_fullpath(const char *path);

#endif