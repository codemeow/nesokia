#ifndef NSK_PLATFORM_FORMATH
#define NSK_PLATFORM_FORMATH

/*!
 * Different compilers accept different modifiers for
 * `__attribute__((format(...))).`. While GCC/MinGW support "gnu_printf"; Clang
 * does not. MinGW treats "printf" as "ms_printf", which (e.g.) lacks the 'z'
 * length modifier for size_t/ssize_t. "gnu_printf" enables it, but fails on
 * Clang.
 *
 * This sets macro that select the appropriate format kind per compiler.
 */
#if defined (__GNUC__) && !defined (__clang__)
    #define NSK_PLATFORM_FORMAT   gnu_printf
#else
    #define NSK_PLATFORM_FORMAT   printf
#endif

#endif