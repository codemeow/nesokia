#ifndef NSK_UTIL_SIZE
#define NSK_UTIL_SIZE

/*!
 * \brief  Provides the number of elements in the STATIC array-like object
 *
 * \param  x The object
 * \return size_t number of elements
 */
#define NSK_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#endif