#!/bin/sh

exec cloc \
     --force-lang-def="$(dirname "${0}")/cloc.def" \
     "$@"