#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'USAGE'
Usage:
  nsk_conv_file2exp.sh input.txt

Parses sections:
  Left:
    - $06 = 0
  Right:
    - $bf = 0

Outputs query-string like:
  l06=0&l07=0&...&r06=0&...

Rules:
  - Only lines inside Left:/Right: are converted.
  - Address must be exactly 2 hex digits ($00..$ff), any case.
  - Value is digits (0-3).
  - Comment lines ("# ...") are ignored
USAGE
}

file="${1:-}"
content=""
if [[ -n "$file" ]]; then
  if [[ ! -f "$file" ]]; then
    echo "File not found: $file" >&2
    exit 1
  fi
  content="$(cat -- "$file")"
else
    echo "Provide file to convert" >&2
    usage
    exit 1
fi

section=""

tolower_hex2() {
  local h="$1"
  printf '%s' "${h,,}"
}

out=""
append_pair() {
  local pair="$1"
  if [[ -z "$out" ]]; then
    out="$pair"
  else
    out+="&$pair"
  fi
}

while IFS= read -r line; do
  case "$line" in
    Left:*)  section="l"; continue ;;
    Right:*) section="r"; continue ;;
  esac

  [[ -z "$section" ]] && continue

  if [[ "$line" =~ \$?([0-9A-Fa-f]{2})[[:space:]]*=[[:space:]]*([0-3]+) ]]; then
    hh="$(tolower_hex2 "${BASH_REMATCH[1]}")"
    val="${BASH_REMATCH[2]}"
    append_pair "${section}${hh}=${val}"
  fi
done <<< "$content"

printf '%s' "$out"

