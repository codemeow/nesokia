#include <getopt.h>

#include "../arguments/nsk_args_options.h"

#include "../arguments/processors/nsk_option_filter.h"
#include "../arguments/processors/nsk_option_follow.h"
#include "../arguments/processors/nsk_option_help.h"
#include "../arguments/processors/nsk_option_quiet.h"
#include "../arguments/processors/nsk_option_recursive.h"
#include "../arguments/processors/nsk_option_table.h"
#include "../arguments/processors/nsk_option_version.h"

/*!
 * Available options table
 */
struct nsk_options_entry nsk_options_table[] = {
   {
        "filter", 'f', required_argument,
        nsk_option_filter,

        "Filter\n"
        "\n"
        "Use a single string to filter ROM entries. The string is a sequence of `pair`s\n"
        "joined by `&`. Only ROMs that satisfy all specified conditions will be displayed\n"
        "\n"
        "Grammar\n"
        "\n"
        "    expr  ::= pair ('&' pair)*\n"
        "    pair  ::= key op value\n"
        "    op    ::= '=' | '!=' | <' | '>' | '<=' | '>='\n"
        "    key   ::= [A-Za-z0-9_]+\n"
        "    value ::= uint32 (decimal)\n"
        "\n"
        "Semantics\n"
        "\n"
        "    * \"=\"  — exact equality\n"
        "    * \"!=\" — not equal\n"
        "    * \"<\"  — strictly less than\n"
        "    * \"<=\" — less or equal\n"
        "    * \">\"  — strictly greater than\n"
        "    * \">=\" — greater or equal\n"
        "    * No spaces; `&` separates conditions.\n"
        "\n"
        "Supported keys\n"
        "\n"
        "         key             values            description\n"
        "    isnes20               0..1      iNES or NES 2.0 header format\n"
        "    console               0..12     Console type: NES, VS System, etc\n"
        "    region                0..3      Console region/timing (NTSC, PAL, etc)\n"
        "    mirroring             0..1      Vertical or Horizontal mirroring\n"
        "    alternative           0..1      Normal or alternative layout\n"
        "    mapper                0..4095   Mapper ID\n"
        "    submapper             0..15     Submapper ID\n"
        "    prg_rom               uint32    PRG ROM size\n"
        "    prg_ram               uint32    PRG RAM size\n"
        "    prg_nvram             uint32    PRG NVRAM size\n"
        "    chr_rom               uint32    CHR ROM size\n"
        "    chr_ram               uint32    CHR RAM size\n"
        "    chr_nvram             uint32    CHR NVRAM size\n"
        "    misc_roms             0..3      Miscellaneous ROMs count\n"
        "    battery               0..1      Battery NVRAM presence flag\n"
        "    trainer               0..1      Trainer area presence flag\n"
        "    device                0..78     Default expansion device\n"
        "    vs_ppu                0..11     Vs. System PPU type\n"
        "    vs_hardware           0..6      Vs. System hardware type\n"
        "\n"
        "Examples\n"
        "\n"
        "    * \"mapper_id=305&prg_ram>0\"\n"
        "        Mapper ID is exactly 305 and PRG RAM size is > 0.\n"
        "    * \"console_region=1&chr_rom=0\"\n"
        "        PAL region/timings and no CHR ROM.\n"
        "    * \"prg_rom>262144&battery_nvram>0\"\n"
        "        PRG ROM larger than 256 KiB and battery-backed NVRAM present.\n"
        "    * \"isnes20=1&submapper_id>0\"\n"
        "        iNES 2.0 with a nonzero submapper.\n"
    },
    {
        "follow", 'l', no_argument,
        nsk_option_follow,
        "Follow symbolic links.\n"
    },
    {
        "help", 'h', no_argument,
        nsk_option_help,
        "Print help and exit\n"
    },
    {
        "quiet", 'q', no_argument,
        nsk_option_quiet,
        "Suppress any output but errors\n"
    },
    {
        "recursive", 'r', no_argument,
        nsk_option_recursive,
        "Enable recursive directory scanning.\n"
        "When specified, the program will process not only the given file(s) or directory\n"
        "but also all subdirectories."
    },
    {
        "table", 't', optional_argument,
        nsk_option_table,
        "Enable table output mode.\n"
        "\n"
        "Instead of detailed per-ROM information, prints a summary table where:\n"
        "\n"
        "* Each row corresponds to a ROM file name.\n"
        "* Each column corresponds to all field names or only optionally \n"
        "  requested ones (e.g. mapper_id, prg_ram, etc.).\n"
        "* The header row starts with the # character.\n"
        "* Fields are separated by the | character.\n"
        "\n"
        "This mode is convenient for quick comparison and scripting.\n"
    },
    {
        "version", 'v', no_argument,
        nsk_option_version,
        "Print program version and exit\n"
    },
};

/*!
 * Available options table size
 */
size_t nsk_options_count =
    sizeof(nsk_options_table) /
    sizeof(nsk_options_table[0]);

/*!
 * \brief  Provided program options
 */
struct nsk_options_program nsk_options_program = { 0 };
