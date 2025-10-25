#ifndef NSK_UTIL_MULTIMAP
#define NSK_UTIL_MULTIMAP

/*!
 * \brief  Multimap type
 */
struct nsk_multimap {
    int         key;    /*!< Key of the record (non-uniq)   */
    const char *value;  /*!< Value of the record            */
};

/*!
 * \brief  Lookups the values from the multimap
 *
 * Returns the merged string with the \p separator as the values separator
 *
 * \param[in] multimap  The multimap
 * \param[in] key       The key
 * \param[in] separator The values separator
 * \return Static string with merged values or the empty string
 */
const char *nsk_multimap_lookup(
    const struct nsk_multimap *multimap,
    int key,
    const char *separator
);

#endif