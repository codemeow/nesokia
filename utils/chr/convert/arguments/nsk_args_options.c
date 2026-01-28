#include <getopt.h>
#include <nsk_util_meta.h>

#include "../arguments/nsk_args_options.h"
#include "../arguments/processors/nsk_option_mode.h"
#include "../arguments/processors/nsk_option_back_address.h"
#include "../arguments/processors/nsk_option_input.h"
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
