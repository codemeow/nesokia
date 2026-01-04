#include <getopt.h>
#include <nsk_util_meta.h>

#include "../arguments/nsk_args_options.h"
#include "../arguments/processors/nsk_option_ansi.h"
#include "../arguments/processors/nsk_option_help.h"
#include "../arguments/processors/nsk_option_palback.h"
#include "../arguments/processors/nsk_option_palmerged.h"
#include "../arguments/processors/nsk_option_palpng.h"
#include "../arguments/processors/nsk_option_palsprites.h"
#include "../arguments/processors/nsk_option_colors.h"
#include "../arguments/processors/nsk_option_ppupng.h"
#include "../arguments/processors/nsk_option_quiet.h"
#include "../arguments/processors/nsk_option_version.h"

/*!
 * Available options table
 */
struct nsk_options_entry nsk_options_table[] = {
    /* Input options */
    {
        "ppu-colors", 'c', required_argument,
        nsk_option_colors,
        "Provides input PPU file as .pal palette file.\n"
        "\n"
        "Format\n"
        "    For the format description visit https://www.nesdev.org/wiki/.pal \n"
        "    In short, it should be 192-1536 bytes long binary file, containing at least\n"
        "    64 3-bytes entries where each entry is an intesity of the color components as follows:\n"
        "     - red   [unsigned 8-bit value: 0..255]\n"
        "     - green [unsigned 8-bit value: 0..255]\n"
        "     - blue  [unsigned 8-bit value: 0..255]\n"
        "\n"
        "Notes\n"
        "    * As the purpose of this program is to convert the non-modified PPU colors list\n"
        "      into PNG file particularly for the `nsk-chr-convert` utility, only first 64\n"
        "      records are used.\n"
        "    * Special value \"dummy\" could be provided to represent empty list\n"
        "\n"
    },
    {
        "pallette-background", 'b', required_argument,
        nsk_option_palback,
        "Provides input palette file for the background palette.\n"
        "\n"
        "Format\n"
        "    The list of the colors, picked from the PPU palette in the following order:\n"
        "    - Palette0: color 0, color 1, color 2, color 3\n"
        "    - Palette1: color 0, color 1, color 2, color 3\n"
        "    - Palette2: color 0, color 1, color 2, color 3\n"
        "    - Palette3: color 0, color 1, color 2, color 3\n"
        "    Thus having 16 records, where the very first record is also a backdrop color.\n"
        "    Each record is an unsigned 8-bit value, representing an index from the `ppu-colors`\n"
        "    palette. Thus requiring `ppu-colors` option to be set.\n"
        "\n"
        "Notes\n"
        "    * This file represents the bytes, that usually load at [$3f00..$3f0f]\n"
        "    * If this option is set, the `palette-sprites` option should also be set.\n"
        "    * Special value \"dummy\" could be provided to represent empty palette. No checks\n"
        "      will be made against the validity this way.\n"
        "    * This option conflicts with `palette-merged`\n"
        "\n"
    },
    {
        "pallette-sprites", 's', required_argument,
        nsk_option_palsprites,
        "Provides input palette file for the sprites palette.\n"
        "\n"
        "Format\n"
        "    See the description of `pallette-background` for the format details.\n"
        "\n"
        "Notes\n"
        "    * This file represents the bytes, that usually load at [$3f10..$3f1f]\n"
        "    * If this option is set, the `palette-background` option should also be set.\n"
        "    * Special value \"dummy\" could be provided to represent empty palette. No checks\n"
        "      will be made against the validity this way.\n"
        "    * This option conflicts with `palette-merged`\n"
        "\n"
    },
    {
        "palette-merged", 'm', required_argument,
        nsk_option_palmerged,
        "Provides input palette file for both background and sprites palette\n"
        "\n"
        "Format\n"
        "    See the description of `pallette-background` for the format details.\n"
        "    This option allows to load both background and sprites palettes with one\n"
        "    merged 32-bytes file\n"
        "\n"
        "Notes\n"
        "    * This file represents the bytes, that usually load at [$3f00..$3f1f]\n"
        "    * Special value \"dummy\" could be provided to represent empty palette. No checks\n"
        "      will be made against the validity this way.\n"
        "    * This options conflicts with `palette-background` and `palette-sprites`\n"
        "\n"
    },
    /* Output options */
    {
        "output-ppu-png", 'C', required_argument,
        nsk_option_ppupng,
        "Save the PPU palette as PNG file.\n"
        "\n"
    },
    {
        "output-palette-png", 'P', required_argument,
        nsk_option_palpng,
        "Save the local background and sprites palette as PNG file.\n"
        "\n"
    },
    {
        "ansi", 'A', no_argument,
        nsk_option_ansi,
        "Show the provided .pal file as ANSI-colored output.\n"
        "\n"
    },
    /* Common options */
    {
        "quiet", 'q', no_argument,
        nsk_option_quiet,
        "Suppress any output but errors\n"
        "\n"
    },
    {
        "help", 'h', no_argument,
        nsk_option_help,
        "Print help and exit\n"
        "\n"
        "Shows usage and option reference.\n"
        "\n"
    },
    {
        "version", 'v', no_argument,
        nsk_option_version,
        "Print program version and exit\n"
        "\n"
    }
};


/*!
 * Available options table size
 */
size_t nsk_options_count = NSK_SIZE(nsk_options_table);

/*!
 * \brief  Provided program options
 */
struct nsk_options_program nsk_options_program = { 0 };

