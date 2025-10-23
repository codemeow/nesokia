#if defined(NSK_OS_GNULINUX)

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*!
 * \brief  Executes platform-specific `stat`
 *
 * \param[in] path     The path
 * \param[out] statbuf  The statbuf
 * \return 0 at success
 */
int nsk_io_stat(const char *path, struct stat *statbuf) {
    return stat(path, statbuf);
}

/*!
 * \brief  Provides platform-specific lstat
 *
 * \param[in]  path  The entry path
 * \param[out] stat  The stat buffer
 * \return 0 at success
 */
int nsk_io_lstat(const char *path, struct stat *statbuf) {
    return lstat(path, statbuf);
}

#endif