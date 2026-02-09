#include <getopt.h>
#include <nsk_util_meta.h>

#include "../arguments/nsk_args_options.h"
#include "../arguments/processors/nsk_option_mode.h"
#include "../arguments/processors/nsk_option_back_address.h"
#include "../arguments/processors/nsk_option_input.h"
#include "../arguments/processors/nsk_option_input_patpal_explicit.h"
#include "../arguments/processors/nsk_option_input_ppu_colors.h"
#include "../arguments/processors/nsk_option_input_palettes.h"
#include "../arguments/processors/nsk_option_input_palette_back.h"
#include "../arguments/processors/nsk_option_input_palette_sprites.h"
#include "../arguments/processors/nsk_option_input_pattern_tables.h"
#include "../arguments/processors/nsk_option_input_left_table.h"
#include "../arguments/processors/nsk_option_input_right_table.h"
#include "../arguments/processors/nsk_option_output.h"
#include "../arguments/processors/nsk_option_output_ppu_colors.h"
#include "../arguments/processors/nsk_option_output_palettes.h"
#include "../arguments/processors/nsk_option_output_palette_back.h"
#include "../arguments/processors/nsk_option_output_palette_sprites.h"
#include "../arguments/processors/nsk_option_output_pattern_tables.h"
#include "../arguments/processors/nsk_option_output_left_table.h"
#include "../arguments/processors/nsk_option_output_right_table.h"
#include "../arguments/processors/nsk_option_help.h"
#include "../arguments/processors/nsk_option_quiet.h"
#include "../arguments/processors/nsk_option_version.h"

/*!
 * Available options table
 */
struct nsk_options_entry nsk_options_table[] = {
    /* Mode options */
    {
        "mode", 'M', required_argument,
        nsk_option_mode,
        "Selects conversion mode.\n"
        "\n"
        "Format\n"
        "    Supported modes:\n"
        "     - t2c (template to CHR)\n"
        "     - c2t (CHR to template)\n"
        "\n"
        "Notes\n"
        "    * Default is `t2c`.\n"
        "\n"
    },
    {
        "back-address", 'A', required_argument,
        nsk_option_back_address,
        "Sets background pattern table address.\n"
        "\n"
        "Format\n"
        "    Supported values:\n"
        "     - 0000\n"
        "     - 1000\n"
        "\n"
        "Notes\n"
        "    * `0000` assigns left table to background.\n"
        "    * `1000` assigns right table to background.\n"
        "\n"
    },

    /* Input options */
    {
        "input", 'i', required_argument,
        nsk_option_input,
        "Provides input combined PNG template file.\n"
        "\n"
        "t2c mode\n"
        "    * Full PNG template file\n"
        "Notes\n"
        "    * Mutually exclusive with separate template components.\n"
        "\n"
    },
    {
        "input-ppu-colors", 'c', required_argument,
        nsk_option_input_ppu_colors,
        "Provides input PPU colors file.\n"
        "\n"
        "t2c mode\n"
        "    * PNG component\n"
        "c2t mode\n"
        "    * PPU colors palette (.pal)\n"
        "\n"
    },
    {
        "input-palettes", 'p', required_argument,
        nsk_option_input_palettes,
        "Provides input palettes file.\n"
        "\n"
        "t2c mode\n"
        "    * PNG component\n"
        "c2t mode\n"
        "    * Combined palettes (`$3f00–$3f1f`)\n"
        "Notes\n"
        "    * Mutually exclusive with `--input-palette-back` and\n"
        "      `--input-palette-sprites`.\n"
        "\n"
    },
    {
        "input-palette-back", 'b', required_argument,
        nsk_option_input_palette_back,
        "Provides input background palette file.\n"
        "\n"
        "c2t mode\n"
        "    * Background palette (`$3f00–$3f0f`)\n"
        "Notes\n"
        "    * Mutually exclusive with `--input-palettes`.\n"
        "\n"
    },
    {
        "input-palette-sprites", 's', required_argument,
        nsk_option_input_palette_sprites,
        "Provides input sprites palette file.\n"
        "\n"
        "c2t mode\n"
        "    * Sprites palette (`$3f10–$3f1f`)\n"
        "Notes\n"
        "    * Mutually exclusive with `--input-palettes`.\n"
        "\n"
    },
    {
        "input-pattern-tables", 't', required_argument,
        nsk_option_input_pattern_tables,
        "Provides input pattern tables file.\n"
        "\n"
        "c2t mode\n"
        "    * Both pattern tables (CHR `$0000–$1fff`) \n"
        "Notes\n"
        "    * Mutually exclusive with `--input-left-table` and\n"
        "      `--input-right-table`.\n"
        "\n"
    },
    {
        "input-left-table", 'l', required_argument,
        nsk_option_input_left_table,
        "Provides input left pattern table file.\n"
        "\n"
        "t2c mode\n"
        "    * PNG component\n"
        "c2t mode\n"
        "    * Left pattern table (CHR `$0000–$0fff`) \n"
        "Notes\n"
        "    * Mutually exclusive with `--input-pattern-tables`.\n"
        "\n"
    },
    {
        "input-right-table", 'r', required_argument,
        nsk_option_input_right_table,
        "Provides input right pattern table file.\n"
        "\n"
        "t2c mode\n"
        "    * PNG component\n"
        "c2t mode\n"
        "    * Right pattern table (CHR `$1000–$1fff`) \n"
        "Notes\n"
        "    * Mutually exclusive with `--input-pattern-tables`.\n"
        "\n"
    },
    {
        "explicit-palettes", 'e', required_argument,
        nsk_option_input_patpal_explicit,
        "Explicitly request a palette index for selected tiles.\n"
        "\n"
        "In some cases an exact palette match cannot be determined. This can occur when\n"
        "a tile uses a subset of colors that is present in more than one palette. In such\n"
        "cases the converter verifies whether these colors have identical indexes across\n"
        "the candidate palettes. If the indexes do not match the converter emits an error\n"
        "and requires an explicitly specified palette index for the tile.\n"
        "\n"
        "Example\n"
        "\n"
        "   - A tile contains two colors: black and white.\n"
        "   - Palette #0 contains black white pink and yellow.\n"
        "   - Palette #2 contains pink blue white and black.\n"
        "\n"
        "Because both palettes include black and white the converter evaluates whether\n"
        "each choice produces the same encoding:\n"
        "\n"
        "   - With palette #0 black is encoded as %00 and white as %01.\n"
        "   - With palette #2 black is encoded as %11 and white as %10.\n"
        "\n"
        "Since the encodings differ the converter requires an explicit palette index for\n"
        "this tile. For example if the tile belongs to the left table and has index $7a\n"
        "the explicit palette specification is as follows:\n"
        "\n"
        "   --explicit-palettes=\"l7a=2\"\n"
        "\n"
        "This specifies palette #2 for the tile.\n"
        "\n"
        "t2c mode\n"
        "    * String, containing the list of tiles and palette indexes\n"
        "\n"
        "Grammar\n"
        "\n"
        "    expr  ::= pair ('&' pair)*\n"
        "    pair  ::= key op value\n"
        "    op    ::= '='\n"
        "    key   ::= [lr][0-f][0-f]\n"
        "    value ::= 0..3 (decimal)\n"
        "\n"
        "Key syntax\n"
        "\n"
        "    * 'l'/'r' - Left or right pattern table\n"
        "    * 00..ff  - Index of the tile in hex format\n"
        "\n"
        "Value syntax\n"
        "\n"
        "    * 0..3    - Index of the requested palette\n"
        "\n"
        "Examples\n"
        "\n"
        "    * \"l24=3&rab=0\"\n"
        "      - Request palette #3 for the $24 tile of the left pattern table\n"
        "      - Request palette #0 for the $ab tile of the right pattern table\n"
        "    * \"l00=3\"\n"
        "      - Request palette #0 for the $00 tile of the left pattern table\n"
        "\n"
    },

    /* Output options */
    {
        "output", 'O', required_argument,
        nsk_option_output,
        "Provides output combined PNG template file.\n"
        "\n"
        "c2t mode\n"
        "    * PNG full template\n"
        "\n"
    },
    {
        "output-ppu-colors", 'C', required_argument,
        nsk_option_output_ppu_colors,
        "Provides output PPU colors file.\n"
        "\n"
        "t2c mode\n"
        "    * PPU colors palette (.pal)\n"
        "c2t mode\n"
        "    * PNG component\n"
        "\n"
    },
    {
        "output-palettes", 'P', required_argument,
        nsk_option_output_palettes,
        "Provides output palettes file.\n"
        "\n"
        "t2c mode\n"
        "    * Combined palettes (`$3f00–$3f1f`)\n"
        "c2t mode\n"
        "    * PNG component\n"
        "\n"
    },
    {
        "output-palette-back", 'B', required_argument,
        nsk_option_output_palette_back,
        "Provides output background palette file.\n"
        "t2c mode\n"
        "    * Background palette (`$3f00–$3f0f`)\n"
        "\n"
    },
    {
        "output-palette-sprites", 'S', required_argument,
        nsk_option_output_palette_sprites,
        "Provides output sprites palette file.\n"
        "t2c mode\n"
        "    * Sprites palette (`$3f10–$3f1f`)\n"
        "\n"
    },
    {
        "output-pattern-tables", 'T', required_argument,
        nsk_option_output_pattern_tables,
        "Provides output pattern tables file.\n"
        "\n"
        "t2c mode\n"
        "    * Both pattern tables (CHR `$0000–$1fff`) \n"
        "\n"
    },
    {
        "output-left-table", 'L', required_argument,
        nsk_option_output_left_table,
        "Provides output left pattern table file.\n"
        "\n"
        "c2t mode\n"
        "    * PNG component\n"
        "t2c mode\n"
        "    * Left pattern table (CHR `$0000–$0fff`) \n"
        "\n"
    },
    {
        "output-right-table", 'R', required_argument,
        nsk_option_output_right_table,
        "Provides output right pattern table file.\n"
        "\n"
        "c2t mode\n"
        "    * PNG component\n"
        "t2c mode\n"
        "    * Left pattern table (CHR `$0000–$0fff`) \n"
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
