#ifndef NSK_UTIL_META
#define NSK_UTIL_META

/*
 * Autogeneration:

find . -name "*.h" | sort | awk '
{
  line = "#include \"" $0 "\""
  lines[NR] = line
  if (length(line) > max) max = length(line)
}
END {
  for (i = 1; i <= NR; i++)
    printf "%-*s // IWYU pragma: export\n", max, lines[i]
}'

 */

#include "./error/nsk_util_errno.h"                  // IWYU pragma: export
#include "./io/nsk_io_dirname.h"                     // IWYU pragma: export
#include "./io/nsk_io_fopen.h"                       // IWYU pragma: export
#include "./io/nsk_io_fullpath.h"                    // IWYU pragma: export
#include "./io/nsk_io_mkdir.h"                       // IWYU pragma: export
#include "./io/nsk_io_mkdirp.h"                      // IWYU pragma: export
#include "./io/nsk_io_opendir.h"                     // IWYU pragma: export
#include "./io/nsk_io_path.h"                        // IWYU pragma: export
#include "./io/nsk_io_readdir.h"                     // IWYU pragma: export
#include "./io/nsk_io_stat.h"                        // IWYU pragma: export
#include "./io/nsk_io_testd.h"                       // IWYU pragma: export
#include "./log/nsk_log_err.h"                       // IWYU pragma: export
#include "./log/nsk_log_inf.h"                       // IWYU pragma: export
#include "./math/nsk_math_endianness.h"              // IWYU pragma: export
#include "./nsk_util_assert.h"                       // IWYU pragma: export
#include "./nsk_util_cleanup.h"                      // IWYU pragma: export
#include "./nsk_util_expsize.h"                      // IWYU pragma: export
#include "./nsk_util_hrbytes.h"                      // IWYU pragma: export
#include "./nsk_util_malloc.h"                       // IWYU pragma: export
#include "./nsk_util_max.h"                          // IWYU pragma: export
#include "./nsk_util_min.h"                          // IWYU pragma: export
#include "./nsk_util_multimap.h"                     // IWYU pragma: export
#include "./nsk_util_size.h"                         // IWYU pragma: export
#include "./png/types/nsk_type_pngcompose.h"         // IWYU pragma: export
#include "./png/types/nsk_type_pngimage.h"           // IWYU pragma: export
#include "./png/types/nsk_type_pngpalettes.h"        // IWYU pragma: export
#include "./png/types/nsk_type_pngpattable.h"        // IWYU pragma: export
#include "./png/types/nsk_type_pngppucolors.h"       // IWYU pragma: export
#include "./png/types/nsk_type_pngtile.h"            // IWYU pragma: export
#include "./strings/nsk_strings_ansi.h"              // IWYU pragma: export
#include "./strings/nsk_strings_columns.h"           // IWYU pragma: export
#include "./strings/nsk_strings_strdup.h"            // IWYU pragma: export
#include "./strings/windows/nsk_strings_wide.h"      // IWYU pragma: export
#include "./types/list/nsk_list_type.h"              // IWYU pragma: export
#include "./types/nsk_type_color4.h"                 // IWYU pragma: export
#include "./types/nsk_type_palettes.h"               // IWYU pragma: export
#include "./types/nsk_type_pattable.h"               // IWYU pragma: export
#include "./types/nsk_type_plane.h"                  // IWYU pragma: export
#include "./types/nsk_type_ppucolors.h"              // IWYU pragma: export
#include "./types/nsk_type_tile.h"                   // IWYU pragma: export
#include "./types/pair/nsk_pair_cmp.h"               // IWYU pragma: export
#include "./types/pair/nsk_pair_parse.h"             // IWYU pragma: export
#include "./types/pair/nsk_pair_type.h"              // IWYU pragma: export
#include "./types/pattables/nsk_pattables_tiles.h"   // IWYU pragma: export
#include "./types/ppucolors/nsk_ppucolors_common.h"  // IWYU pragma: export
#include "./types/ppucolors/nsk_ppucolors_readpal.h" // IWYU pragma: export
#include "./types/ppucolors/nsk_ppucolors_saveaco.h" // IWYU pragma: export
#include "./types/ppucolors/nsk_ppucolors_savease.h" // IWYU pragma: export
#include "./types/ppucolors/nsk_ppucolors_savegpl.h" // IWYU pragma: export
#include "./types/ppucolors/nsk_ppucolors_savepal.h" // IWYU pragma: export


#endif