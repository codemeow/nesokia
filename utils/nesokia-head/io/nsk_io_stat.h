#ifndef NSK_IO_STAT
#define NSK_IO_STAT

#include <sys/stat.h>

#if !defined (S_IFLNK)
    /* By some reason sometimes missed */
    #define S_IFLNK 0120000
#endif

#if !defined (S_ISLNK)
    /* By some reason sometimes missed */
    #define S_ISLNK(mode) (((mode) & S_IFMT) == S_IFLNK)
#endif

/*!
 * \brief  Provides platform-specific lstat
 *
 * \param[in]  path  The entry path
 * \param[out] stat  The stat buffer
 * \return 0 at success
 */
int nsk_io_lstat(const char *path, struct stat *stat);

/*!
 * \brief  Executes platform-specific `stat`
 *
 * \param[in] path     The path
 * \param[out] statbuf  The statbuf
 * \return 0 at success
 */
int nsk_io_stat(const char *path, struct stat *statbuf);

#endif
