#ifndef NSK_ROMTYPE_CONSOLE
#define NSK_ROMTYPE_CONSOLE

/*!
 * \brief  List of console types
 *
 * \note Constructed from xconst_console file via XMacro,
 * expected format:
 * ~~~
 * NSK_CONSOLE_NES      = 0,
 * NSK_CONSOLE_FAMICOM  = 0,
 * ...
 * ~~~
 */
enum nsk_console_type {

#   define X(key, name, value, description) \
    NSK_CONSOLE_ ## name = value,

#       include <xconstants/nsk_xconst_console.x>

#   undef X

};

/*!
 * \brief  Returns string description of the console type
 *
 * \param[in] type  The type
 * \return  Static string
 */
const char *nsk_console_name(enum nsk_console_type type);

#endif