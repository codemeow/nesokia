#ifndef NSK_ARGS_OPTIONS
#define NSK_ARGS_OPTIONS

#include <stdlib.h>
#include <stdbool.h>

/*!
 * \brief  The starting point of the short-option replacing value
 *
 * Use this value when the entry lacks of the short option. Increment the value
 * for each entry of this kind
 */
#define NSK_OPTION_SHORTLIMIT (1000)

/*!
 * \brief  Options table entry
 */
struct nsk_options_entry {
    const char   *option_long;             /*!< Long argument option or NULL  */
    const int     option_short;            /*!< Short argument option or 1000+*/
    const int     option_arg;              /*!< Argument presence flag        */
    void        (*option_processor)(void); /*!< Argument processor            */
    const char   *option_desc;             /*!< Argument description          */
};

/*!
 * \brief  List of input formats
 */
enum nsk_option_inputs {
    NSK_OPTION_INPUT_AUTO, /*!< Deduce automatically */
    NSK_OPTION_INPUT_PNG,  /*!< PNG file             */
    NSK_OPTION_INPUT_PAL,  /*!< PAL file             */

    NSK_OPTION_INPUTS_COUNT /*!< Number of formats */
};

/*!
 * \brief  List of output formats
 */
enum nsk_option_outputs {
    NSK_OPTION_OUTPUT_AUTO, /*!< Deduce automatically */
    NSK_OPTION_OUTPUT_PNG,  /*!< PNG file             */
    NSK_OPTION_OUTPUT_PAL,  /*!< PAL file             */
    NSK_OPTION_OUTPUT_GPL,  /*!< GPL file             */
    NSK_OPTION_OUTPUT_ASE,  /*!< ASE file             */
    NSK_OPTION_OUTPUT_ACO,  /*!< ACO file             */

    NSK_OPTION_OUTPUTS_COUNT /*!< Number of formats */
};

/*!
 * \brief  Provided program options
 */
struct nsk_options_program {
    struct {
        const char *file;               /*!< Input file name   */
        enum nsk_option_inputs format;  /*!< Input file format */
    } input; /*!< Input options */
    struct {
        const char *file;               /*!< Output file name   */
        enum nsk_option_outputs format; /*!< Output file format */
    } output; /*!< Output options */

    char *const *files; /*!< Compability list of files */
};

/*!
 * Available options table
 */
extern struct nsk_options_entry nsk_options_table[];

/*!
 * Available options table size
 */
extern size_t nsk_options_count;

/*!
 * \brief  Provided program options
 */
extern struct nsk_options_program nsk_options_program;

/*!
 * \brief  Provides the input format by string definition
 *
 * \param[in] string  The string
 * \return format or _AUTO
 */
enum nsk_option_inputs nsk_input_format(const char *string);

/*!
 * \brief  Provides the output format by string definition
 *
 * \param[in] string  The string
 * \return format or _AUTO
 */
enum nsk_option_outputs nsk_output_format(const char *string);

#endif