#ifndef NSK_IO_PATH
#define NSK_IO_PATH

#if defined(NSK_OS_WINDOWS)
    #include "../../utils/io/windows/nsk_io_path.h"      // IWYU pragma: export

#elif defined(NSK_OS_GNULINUX)
    #include <linux/limits.h>
    #include "../../utils/io/gnulinux/nsk_io_path.h"     // IWYU pragma: export
#endif

#endif