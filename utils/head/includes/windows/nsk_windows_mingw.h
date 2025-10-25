#ifndef NSK_WINDOWS_MINGW
#define NSK_WINDOWS_MINGW

/*
 * Older MinGW versions (e.g. 9.3) don’t define this macro by default,
 * causing PRIx macros to expand into formats unsupported by gnu_printf.
 */
#if !defined(__USE_MINGW_ANSI_STDIO)
    #define __USE_MINGW_ANSI_STDIO 1
#endif

#endif