#include <getopt.h>
#include <nsk_util_meta.h>

#include "../arguments/nsk_args_options.h"
#include "../arguments/processors/nsk_option_chrback.h"
#include "../arguments/processors/nsk_option_chrsprites.h"
#include "../arguments/processors/nsk_option_colors.h"
#include "../arguments/processors/nsk_option_help.h"
#include "../arguments/processors/nsk_option_left.h"
#include "../arguments/processors/nsk_option_outdirectory.h"
#include "../arguments/processors/nsk_option_palback.h"
#include "../arguments/processors/nsk_option_palettes.h"
#include "../arguments/processors/nsk_option_palsprites.h"
#include "../arguments/processors/nsk_option_ppuctrl34.h"
#include "../arguments/processors/nsk_option_quiet.h"
#include "../arguments/processors/nsk_option_right.h"
#include "../arguments/processors/nsk_option_template.h"
#include "../arguments/processors/nsk_option_terminalansi.h"
#include "../arguments/processors/nsk_option_version.h"

/*!
 * Available options table
 */
struct nsk_options_entry nsk_options_table[] = {
    /* Output directory options */
    {
        "output-directory", 'D', required_argument,
        nsk_option_outdirectory,
        "Set output directory for files\n"
        "\n"
        "By default, all generated files are written to the current directory\n"
        "When this option is specified, output files are written to the given directory.\n"
        "\n"
        "Notes"
        "    * Be aware that using this option along with --name-* options will combine\n"
        "      both components together even if the absolute path is given, for example:\n"
        "\n"
        "      nesokia-chr-convert -D out -B back.chr\n"
        "          Will write the nametable to \"./out/back.chr\"\n"
        "      nesokia-chr-convert -D out -B subdir/back.chr\n"
        "          Will write the nametable to \"./out/subdir/back.chr\"\n"
        "      nesokia-chr-convert -D out -B C:\\back.chr\"\n"
        "          Will wrongly try to write the file as \".\\out\\C:\\back.chr\"\n"
        "      nesokia-chr-convert -D C:\\out -B C:\\back.chr\n"
        "          Will wrongly try to write the file as \"C:\\out\\C:\\back.chr\"\n"
        "\n"
        "Examples\n"
        "\n"
        "    nesokia-chr-convert -D chr image.png\n"
        "        Writes all files into \"chr\" directory.\n"
        "\n"
    },

    /* Explicit output file naming for generated artifacts */
    {
        "name-chr-back", 'B', required_argument,
        nsk_option_chrback,
        "Set output file name for background CHR bank\n"
        "\n"
        "When specified, the background CHR table will be written to the given file.\n"
        "The directory part of the path is used as-is.\n"
        "\n"
        "Group semantics (applies to all --name-* options)\n"
        "\n"
        "    * If none of the --name-* options are used:\n"
        "        - All components are generated.\n"
        "\n"
        "    * If at least one --name-* option is used:\n"
        "        - Only explicitly specified components are generated.\n"
        "        - Validation for missing components is skipped.\n"
        "        - --output-directory option become optional.\n"
        "\n"
        "Notes\n"
        "    * As left and right nametables are not directly linked with background\n"
        "      and sprites planes, the --ppuctrl34 option is used to control that\n"
        "\n"
        "\n"
        "Examples\n"
        "\n"
        "    nesokia-chr-convert -B tiles.chr image.png\n"
        "        Generates only the left CHR bank and writes it to \"tiles.chr\".\n"
        "\n"
    },
    {
        "name-chr-sprites", 'S', required_argument,
        nsk_option_chrsprites,
        "Set output file name for CHR sprites bank\n"
        "\n"
        "When specified, the sprites CHR table will be written to the given file.\n"
        "The directory part of the path is used as-is.\n"
        "\n"
        "See \"name-chr-back\" for group semantics.\n"
        "\n"
        "Examples\n"
        "\n"
        "    nesokia-chr-convert -1 bullets.chr image.png\n"
        "        Generates only the right CHR bank and writes it to \"bullets.chr\".\n"
        "\n"
    },
    {
        "name-palette-back", 'b', required_argument,
        nsk_option_palback,
        "Set output file name for background palette\n"
        "\n"
        "When specified, the background palette will be written to the given file.\n"
        "The directory part of the path is used as-is.\n"
        "\n"
        "See \"name-chr-back\" for group semantics.\n"
        "\n"
        "Examples\n"
        "\n"
        "    nesokia-chr-convert -b tiles.pal image.png\n"
        "        Generates only the background palette and writes it to \"tiles.pal\".\n"
        "\n"
    },
    {
        "name-palette-sprites", 's', required_argument,
        nsk_option_palsprites,
        "Set output file name for sprite palette\n"
        "\n"
        "When specified, the sprite palette will be written to the given file.\n"
        "The directory part of the path is used as-is.\n"
        "\n"
        "See \"name-chr-back\" for group semantics.\n"
        "\n"
        "Examples\n"
        "\n"
        "    nesokia-chr-convert -s sprites.pal image.png\n"
        "        Generates only the sprite palette and writes it to \"sprites.pal\".\n"
        "\n"
    },

    /* Composite-image workflow: assemble final image from separate component images */
    {
        "template", 't', required_argument,
        nsk_option_template,
        "Set base template image\n"
        "\n"
        "Specifies the base image used when not all components are explicitly overridden.\n"
        "Any missing components will be taken from this image.\n"
        "\n"
        "Semantics\n"
        "\n"
        "    * If all required components are provided explicitly:\n"
        "        - --template is optional.\n"
        "\n"
        "    * If at least one required component is missing:\n"
        "        - --template must be specified.\n"
        "        - Missing components are taken from the template image.\n"
        "\n"
        "Examples\n"
        "\n"
        "    nesokia-chr-convert -t image.png -l left.png\n"
        "        Uses \"image.png\" as base, replaces left CHR table with \"left.png\".\n"
        "\n"
    },
    {
        "left", 'l', required_argument,
        nsk_option_left,
        "Set image for left CHR table ($0000)\n"
        "\n"
        "Overrides the left CHR table in the final image.\n"
        "\n"
        "See \"template\" option for rules on missing components.\n"
        "\n"
        "Examples\n"
        "\n"
        "    nesokia-chr-convert -t image.png -l left.png\n"
        "        Replaces only the left CHR table.\n"
        "\n"
    },
    {
        "right", 'r', required_argument,
        nsk_option_right,
        "Set image for right CHR table ($1000)\n"
        "\n"
        "Overrides the right CHR table in the final image.\n"
        "\n"
        "See \"template\" option for rules on missing components.\n"
        "\n"
        "Examples\n"
        "\n"
        "    nesokia-chr-convert -t image.png -r right.png\n"
        "        Replaces only the right CHR table.\n"
        "\n"
    },
    {
        "colors", 'c', required_argument,
        nsk_option_colors,
        "Set image for global palette (colors)\n"
        "\n"
        "Overrides the global PPU palette (color set) in the final image.\n"
        "\n"
        "See \"template\" option for rules on missing components.\n"
        "\n"
        "Examples\n"
        "\n"
        "    nesokia-chr-convert -c ppu/2c02.png image.png\n"
        "        Uses colors from \"ppu/2c02.png\".\n"
        "\n"
    },
    {
        "palettes", 'p', required_argument,
        nsk_option_palettes,
        "Set image for tile palettes\n"
        "\n"
        "Overrides the palettes image that tiles reference/use.\n"
        "\n"
        "See \"template\" option for rules on missing components.\n"
        "\n"
        "Examples\n"
        "\n"
        "    nesokia-chr-convert -p steve/palette.png image.png\n"
        "        Uses tile palettes from \"steve/palette.png\".\n"
        "\n"
    },
    /* Settings */
    {
        "ppuctrl34", 'P', required_argument,
        nsk_option_ppuctrl34,
        "Set the tables order\n"
        "\n"
        "As the nametables can be rearranged via PPUCTRL 3th and 4th bits and the\n"
        "palettes are always fixed this option controls the purpose of the nametables\n"
        "\n"
        "Allowed values\n"
        "\n"
        "|- value -|-  PPUCTRL  -|-               Meaning               -|\n"
        "|  normal | ...0 1...   | Background at $0000, sprites at $1000 |\n"
        "|  invert | ...1 0...   | Background at $1000, sprites at $0000 |\n"
        "\n"
        "See the nametable address on the template file for selection\n"
        "\n"
        "Notes\n"
        " * This does not affect the real load address, just makes the clarification\n"
        "   for this program's validator\n"
        " * Default value is \"normal\"\n"
        "\n"
    },
    {
        "terminal-ansi", 'a', no_argument,
        nsk_option_terminalansi,
        "Use the ANSI codes to color the terminal output\n"
        "\n"
        "If the terminal supports the 24-bit ANSI foreground sequences this mode\n"
        "allowes to show the real color along with the color hex-code\n"
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

