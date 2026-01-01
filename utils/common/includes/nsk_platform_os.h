#ifndef NSK_PLATFORM_OS
#define NSK_PLATFORM_OS

#if defined(_WIN32) || defined(_WIN64) || \
    defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
    /*!
     * Windows detection macro
     */
    #define NSK_OS_WINDOWS
#endif


#if defined(__gnu_linux__)
    /*!
     * GNU/Linux detection macro
     */
    #define NSK_OS_GNULINUX
#endif

#endif