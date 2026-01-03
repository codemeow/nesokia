#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <nsk_util_meta.h>

#include "../../types/pair/nsk_pair_parse.h"
#include "../../types/header/nsk_header_table.h"
#include "../../types/pair/nsk_pair_cmp.h"

/*!
 * In case of error print no more than this number of characters
 */
static const int nsk_output_limit = 20;

/*!
 * \brief  Checks and parses the provided string which starts with correct
 * name (key).
 *
 * Refer to the nsk_header_table.shortcut for the list of values
 *
 * \param[in]  start  The string
 * \param[out] size   The size
 * \return True if the string starts with the correct key
 */
static bool _is_valid_key(const char *start, size_t *size) {
    for (size_t i = 0; i < nsk_header_tablesize; i++) {
        int cmp = strncmp(
            nsk_header_table[i].shortcut,
            start,
            strlen(nsk_header_table[i].shortcut)
        );

        if (cmp == 0) {
            *size = strlen(nsk_header_table[i].shortcut);
            return true;
        }
    }

    return false;
}

/*!
 * \brief  Checks and parses the provided string which starts with correct
 * operator string
 *
 * \param[in]  start      The string
 * \param[out] size       The size
 * \param[out] operator   The parsed operator
 * \return True if the string starts with operator (sub)string
 */
static bool _is_valid_operator(
    const char *start,
    size_t *size,
    enum nsk_pair_operator *operator
) {
    return nsk_pair_isoperator(start, size, operator);
}

/*!
 * \brief  Checks and parses the provided string which starts with correct
 * uint64_t value
 *
 * \param[in]  start  The string
 * \param[out] size   The size
 * \param[out] value  The parsed value
 * \return True if the string starts with uint64_t value
 */
static bool _is_valid_value(const char *start, size_t *size, uint64_t *value) {
    const uint64_t cut = UINT64_MAX / 10;
    const uint64_t rem = UINT64_MAX % 10;

    const char *orig = start;

    uint64_t result = 0;
    while(*start) {
        if (*start == '&' || *start == '\0') {
            break;
        }

        if (isdigit((unsigned char)*start) == 0) {
            nsk_err(
                "Error: bad character '%c' in value \"...%.*s\"\n",
                *start,
                nsk_output_limit,
                start
            );
            return false;
        }

        unsigned char digit = (unsigned char)*start - '0';

        if (result > cut || (result == cut && digit > rem)) {
            nsk_err(
                "Error: value \"...%.*s\" is too large\n",
                nsk_output_limit,
                start
            );
            return false;
        }

        result = result * 10 + digit;

        start++;
    }

    *value = result;
    *size  = start - orig;
    return true;
}

/*!
 * \brief  Reads key (name) from the provided string
 *
 * \param[in]  string  The string
 * \param[out] keylen  The key length
 * \param[out] key     The key
 * \return String, shifted to the new (next) position
 */
static const char *_parse_key(const char *string, size_t *keylen, const char **key) {
    const char *value = string;
    size_t      size;
    if (!_is_valid_key(value, &size)) {
        nsk_err(
            "Error: bad key at \"...%.*s\"\n",
            nsk_output_limit,
            string
        );
        return NULL;
    }

    *key    = value;
    *keylen = size;
    return string + size;
}

/*!
 * \brief  Reads operator from the provided string
 *
 * \param[in]  string     The string
 * \param[out] operator   The operator
 * \return String, shifted to the new (next) position
 */
static const char *_parse_operator(const char *string, enum nsk_pair_operator *operator) {
    size_t size;
    if (!_is_valid_operator(string, &size, operator)) {
        nsk_err(
            "Error: bad operator at \"...%.*s\"\n",
            nsk_output_limit,
            string
        );
        return NULL;
    }

    return string + size;
}

/*!
 * \brief  Reads value from the provided string
 *
 * \param[in]  string  The string
 * \param[out] value   The value
 * \return String, shifted to the new (next) position
 */
static const char *_parse_value(const char *string, uint64_t *value) {
    size_t size;
    if (!_is_valid_value(string, &size, value)) {
        nsk_err(
            "Error: bad value at \"...%.*s\"\n",
            nsk_output_limit,
            string
        );
        return NULL;
    }

    return string + size;
}

/*!
 * \brief  Parses the provided string into pairs and appends it to the parent
 *
 * \param[in] string  The string
 * \param     parent  The parent
 * \param[in] fields  The combination of nsk_pair_fields
 * \return    Allocated list of pairs
 */
static struct nsk_pair *_parse_pair(
    const char      *string,
    struct nsk_pair *parent,
    unsigned         fields
) {
    const char             *key      = "";
    size_t                  keylen   = 0;
    enum nsk_pair_operator  operator = NSK_PAIR_EQUAL;
    uint64_t                value    = 0;

    if ((fields & NSK_PAIR_NAME) > 0) {
        string = _parse_key(string, &keylen, &key);
        if (!string) {
            return NULL;
        }
    }

    if ((fields & NSK_PAIR_OPERATOR) > 0) {
        string = _parse_operator(string, &operator);
        if (!string) {
            return NULL;
        }
    }

    if ((fields & NSK_PAIR_VALUE) > 0) {
        string = _parse_value(string, &value);
        if (!string) {
            return NULL;
        }
    }

    struct nsk_pair *pair = nsk_pair_appendn(
        key,
        keylen,
        value,
        operator,
        parent
    );

    if (*string == '&') {
        if (!_parse_pair(string + 1, pair, fields)) {
            pair->list.next = NULL;
            nsk_pair_free(pair);
            return NULL;

        } else {
            return pair;
        }

    } else if (*string == '\0') {
        return pair;

    } else {
        nsk_pair_free(pair);
        nsk_err(
            "Error: bad delimeter at \"...%.*s\"\n",
            nsk_output_limit,
            string
        );
        return NULL;
    }
}

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
 * \return  Allocated list of pairs
 */
struct nsk_pair *nsk_pair_parse(const char *string, unsigned fields) {
    struct nsk_pair *pair = _parse_pair(string, NULL, fields);
    if (!pair) {
        nsk_err("Error: cannot parse list string: \"%s\"\n", string);
        exit(EXIT_FAILURE);
    }
    return pair;
}