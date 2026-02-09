#ifndef NSK_PAIR_PARSE
#define NSK_PAIR_PARSE

#include "../../types/pair/nsk_pair_type.h"

/*!
 * \brief  Parse the string into a list of pairs
 *
 * Expects the following syntax:
 *
 * Grammar:
 *
 *     expr  ::= pair ('&' pair)
 *     pair  ::= key op value
 *     op    ::= '=' | '!=' | <' | '>' | '<=' | '>='
 *     key   ::= [A-Za-z0-9_]+
 *     value ::= uint64 (decimal)
 *
 * Examples:
 *
 * * "mapper>304&prgrom!=1024"
 * * "device!=0&battery=0&trainer=0&console=3"
 * * "prgrom>2048&prgram>=1024&prg_nvram!=0"
 *
 * Refer to the (struct nsk_header_tableentry).shortcut for the list of
 * available key values
 *
 * \param[in] string  The input string
 * \param[in] fields  The combination of nsk_pair_fields to indicate which
 *                    fields to parse
 * \param[in] keys    List of string values representing valid keys or NULL
 * \return  Allocated list of pairs
 */
struct nsk_pair *nsk_pair_parse(
    const char *string,
    unsigned fields,
    const char *keys[]
);

#endif