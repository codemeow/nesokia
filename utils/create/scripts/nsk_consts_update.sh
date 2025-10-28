#!/bin/sh

DIRECTORY_COMMON=../../common
DIRECTORY_XCONSTS=${DIRECTORY_COMMON}/xconstants

cd "$(dirname "${0}")" || exit 1

# @brief Converts 4 columns table
# $1 Filename
# $2 Brief header
# $3 Scope name
#
# Output:
#     Prints converted enum
nsk_convert_4cols() {
    printf "    ; @brief ${2}\n"
    printf "    .scope ${3}\n"
    awk -F'[(),"]+' '/^X/ {
        gsub(/^ +| +$/, "", $3);
        gsub(/^ +| +$/, "", $4);
        gsub(/^ +| +$/, "", $5);
        printf("        ; @brief %s\n", $6);
        printf("        %s = %s\n\n", $3, $4);
    }' ${1}
    printf "    .endscope ; %s\n\n" "${3}"
}

# @brief Converts 6 columns table
# $1 Filename
# $2 Brief header
# $3 Scope name
#
# Output:
#     Prints converted enum
nsk_convert_6cols() {
    printf "    ; @brief ${2}\n"
    printf "    .scope ${3}\n"
    awk -F'[(),"]+' '/^X/ {
        gsub(/^ +| +$/, "", $3);
        gsub(/^ +| +$/, "", $4);
        gsub(/^ +| +$/, "", $5);
        gsub(/^ +| +$/, "", $6);
        printf("        ; @brief %s\n", $7);
        printf("        %s_%s = %s\n\n", $3, $4, $5);
    }' ${1}
    printf "    .endscope ; %s\n\n" "${3}"
}

#@brief Converts single file
# $1 Filename
# $2 Single line, containing
#     - Brief header
#     - Scope name
#     - Columns number
#
# Output:
#     Prints converted enum
nsk_convert_file() {
    filename="$1"

    oldIFS="${IFS}"
    IFS='
'
    set -- $2
    IFS="${oldIFS}"

    brief="$1"
    scope="$2"
    cols="$3"

    case ${cols} in
        *[!0-9]*|'')
            printf 'ERR: %s: Columns is not a number: %s\n' "${filename}" "${cols}" >&2
            exit 1
            ;;
    esac

    case "${cols}" in
        4) nsk_convert_4cols "${filename}" "${brief}" "${scope}" ;;
        6) nsk_convert_6cols "${filename}" "${brief}" "${scope}" ;;
        *) printf 'ERR: %s: Columns is not processable: %s\n' "${filename}" "${cols}" >&2 ;;
    esac
}

# @brief Extracts metadata from the xconstants files
# $1 Filename
#
# Output:
#     Prints Header\nScope\nColumns\n
nsk_metadata_extract() {
    awk '
        BEGIN{ header=""; scope=""; cols="" }
        /^[[:space:]]*\/\/[[:space:]]*\[[[:space:]]*Header:/ {
            if (match($0,/"[^"]*"/)) {
                s=substr($0, RSTART+1, RLENGTH-2);
                header=s
            }
        }
        /^[[:space:]]*\/\/[[:space:]]*\[[[:space:]]*Scope:/ {
            if (match($0,/"[^"]*"/)) {
                s=substr($0, RSTART+1, RLENGTH-2);
                scope=s
            }
        }
        /^[[:space:]]*\/\/[[:space:]]*\[[[:space:]]*Columns:/ {
            if (match($0,/[0-9]+/)) {
                cols=substr($0, RSTART, RLENGTH)
            }
        }

        END {
            if (header!="" && scope!="" && cols!="")
                printf "%s\n%s\n%s\n", header, scope, cols
        }
    ' "$1"
}

# @brief Scans passed directory and converts xconstants files into ca65-enums
# $1 Directory name
#
# Output:
#     Prints converted enums from every valid file
nsk_convert_directory() {
    find "$1" \( -type f -name '*.x' \) | while IFS= read -r filename; do
        meta=$(nsk_metadata_extract "${filename}") || meta=
        if [ -z "${meta}" ]; then
            printf 'WARN: %s: No metadata, skip\n' "${filename}" >&2
            continue
        fi

        nsk_convert_file "${filename}" "${meta}"
    done
}

nsk_convert_fileheader() {
    printf "; @file nsk_header_consts.inc\n"
    printf "; @brief Constant definitions and enumerations for NES 2.0 headers.\n"
    printf ";\n"
    printf "; Provides named values for flags, and configuration\n"
    printf "; options referenced in \`nsk_header_config.inc\`.\n"
    printf ";\n"
    printf "; Part of the Nesokia project — MIT License.\n"
    printf "\n"
    printf ".ifndef ::__NSK_HEADER_CONSTS__\n"
    printf "::__NSK_HEADER_CONSTS__ = 1\n"
    printf "\n"
}

nsk_convert_filefooter() {
    printf "\n"
    printf ".endif\n"
}

nsk_convert_scopeheader() {
    printf "; @brief Global project scope\n"
    printf ".scope NSK\n"
}

nsk_convert_scopefooter() {
    printf ".endscope ; NSK\n"
}

nsk_convert_fileheader
nsk_convert_scopeheader
nsk_convert_directory ${DIRECTORY_XCONSTS}
nsk_convert_scopefooter
nsk_convert_filefooter
