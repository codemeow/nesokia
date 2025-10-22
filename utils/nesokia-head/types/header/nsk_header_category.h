#ifndef NSK_HEADER_CATEGORY
#define NSK_HEADER_CATEGORY

/*!
 * \brief  Row categories for grouping
 */
enum nsk_table_category {
    NSK_CATEGORY_NONE,          /*!< Initial state              */
    NSK_CATEGORY_FORMAT,        /*!< iNES/NES2.2048             */
    NSK_CATEGORY_CONSOLE,       /*!< Console type/region/etc    */
    NSK_CATEGORY_NAMETABLES,    /*!< Nametables info            */
    NSK_CATEGORY_MAPPER,        /*!< Mapper info                */
    NSK_CATEGORY_MEMORY,        /*!< Memory info (RAM/ROM/NV)   */
    NSK_CATEGORY_MISC           /*!< Miscellaneous info         */
};


/*!
 * \brief  Returns the fancy name of the category
 *
 * \param[in] category  The category
 * \return Static string
 */
const char *nsk_category_name(enum nsk_table_category category);

#endif