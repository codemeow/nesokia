#if defined(NSK_OS_WINDOWS)

#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <windows.h>

#include "../../io/nsk_io_stat.h"

#include "../../../utils/log/nsk_log_err.h"
#include "../../../utils/strings/windows/nsk_strings_wide.h"
#include "../../../utils/nsk_util_cleanup.h"
#include "../../../utils/nsk_util_malloc.h"

/*!
 * \brief  Symlink types list
 */
enum nsk_io_symlink {
    NSK_SYMLINK_NONE,       /*!< Not a symlink          */
    NSK_SYMLINK_FILE,       /*!< Symlink to a file      */
    NSK_SYMLINK_DIRECTORY   /*!< Symlink to a directory */
};

/*!
 * \brief  Checks the symlink type of the path
 *
 * \param[in] path  The path
 * \return Type of the symlink
 */
enum nsk_io_symlink _symlink_type(LPWSTR path) {
    WIN32_FIND_DATAW data;

    HANDLE handle = FindFirstFileW(path, &data);
    if (handle == INVALID_HANDLE_VALUE) {
        return NSK_SYMLINK_NONE;
    }

    FindClose(handle);

    if (
        (data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) == 0 ||
        data.dwReserved0 != IO_REPARSE_TAG_SYMLINK
    ) {
        return NSK_SYMLINK_NONE;
    }

    if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
        return NSK_SYMLINK_DIRECTORY;
    }

    return NSK_SYMLINK_FILE;
}

/*!
 * \brief  Returns symlink's mode
 *
 * \param[in] info  The BHFI information
 * \return Symlink mode
 */
static mode_t _symlink_mode(const BY_HANDLE_FILE_INFORMATION * info) {
    mode_t result = (S_IRUSR | S_IRGRP | S_IROTH);

    if ((info->dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0)
        result |= (S_IWUSR | S_IWGRP | S_IWOTH);

    if ((info->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
        result |= (S_IXUSR | S_IXGRP | S_IXOTH);

    return result;
}

/*!
 * \brief  Converts Windows FILETIME to unix time
 *
 * \param[in]  filetime  The filetime
 * \param[out] unixtime  The unixtime
 * \return True on success
 */
static bool _filetime_convert(const FILETIME * filetime, time_t *unixtime) {
    SYSTEMTIME systime;
    if (!FileTimeToSystemTime(filetime, &systime)) {
        nsk_err("Error: FileTimeToSystemTime error: %lu\n", GetLastError());
        return false;
    }

    SYSTEMTIME localtime;
    if (!SystemTimeToTzSpecificLocalTime(NULL, &systime, &localtime)) {
        nsk_err(
            "Error: SystemTimeToTzSpecificLocalTime error: %lu\n",
            GetLastError()
        );
        return false;
    }

    struct tm filetime_tm = {
        .tm_sec  = localtime.wSecond,
        .tm_min  = localtime.wMinute,
        .tm_hour = localtime.wHour,
        .tm_mday = localtime.wDay,
        .tm_mon  = localtime.wMonth - 1,
        .tm_year = localtime.wYear - 1900
    };

    time_t result = mktime(&filetime_tm);
    if (result == -1) {
        return false;
    }

    *unixtime = result;
    return true;
}

/*!
 * \brief  Provides symlink information
 *
 * \param[in] info      The BHFI information
 * \param[out] statbuf  The statbuf
 * \return True on success
 */
static bool _symlink_stat(
    const BY_HANDLE_FILE_INFORMATION *info,
    struct stat                      *statbuf
) {
    statbuf->st_dev = statbuf->st_rdev = info->dwVolumeSerialNumber;

    statbuf->st_mode  = (S_IFLNK | _symlink_mode(info));
    statbuf->st_nlink = info->nNumberOfLinks;
    statbuf->st_size  = (ULARGE_INTEGER){
        .HighPart = info->nFileSizeHigh,
        .LowPart  = info->nFileSizeLow
    }.QuadPart;

    statbuf->st_gid = statbuf->st_uid = 0;
    statbuf->st_ino = 0;

    return (_filetime_convert(&info->ftLastAccessTime, &statbuf->st_atime) &&
            _filetime_convert(&info->ftCreationTime,   &statbuf->st_ctime) &&
            _filetime_convert(&info->ftLastWriteTime,  &statbuf->st_mtime));
}

/*!
 * \brief  Executes platform-specific `stat`
 *
 * \param[in] path     The path
 * \param[out] statbuf  The statbuf
 * \return 0 at success
 */
int nsk_io_stat(const char *path, struct stat *statbuf) {
    nsk_auto_free LPWSTR wpath = nsk_string_a2w(path);

    int result = wstat(wpath, statbuf);

    return result;
}

/*!
 * \brief  Provides platform-specific lstat
 *
 * \param[in]  path  The entry path
 * \param[out] stat  The stat buffer
 * \return 0 at success
 */
int nsk_io_lstat(const char *path, struct stat *statbuf) {
    nsk_auto_free LPWSTR wpath = nsk_string_a2w(path);

    if (_symlink_type(wpath) == NSK_SYMLINK_NONE) {
        return nsk_io_stat(path, statbuf);
    }

    DWORD flags_share =
        FILE_SHARE_DELETE   |
        FILE_SHARE_READ     |
        FILE_SHARE_WRITE;
    DWORD flags_attr =
        FILE_ATTRIBUTE_NORMAL           |
        FILE_FLAG_BACKUP_SEMANTICS      |
        FILE_FLAG_OPEN_REPARSE_POINT;

    HANDLE handle = CreateFileW(
        wpath,
        0,
        flags_share,
        NULL,
        OPEN_EXISTING,
        flags_attr,
        NULL
    );

    if (handle == INVALID_HANDLE_VALUE) {
        nsk_err("Error: CreateFileW error: %lu\n", GetLastError());
        return -1;
    }

    BY_HANDLE_FILE_INFORMATION info;

    bool success = GetFileInformationByHandle(handle, &info);
    CloseHandle(handle);

    if (!success) {
        nsk_err(
            "Error: GetFileInformationByHandle error: %lu\n",
            GetLastError()
        );
        return -1;
    }

    if (!_symlink_stat(&info, statbuf)) {
        return -1;
    }

    return 0;
}

#endif