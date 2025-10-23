#include <getopt.h>

#include "../arguments/nsk_args_options.h"

#include "../arguments/processors/nsk_option_filter.h"
#include "../arguments/processors/nsk_option_follow.h"
#include "../arguments/processors/nsk_option_help.h"
#include "../arguments/processors/nsk_option_keys.h"
#include "../arguments/processors/nsk_option_output.h"
#include "../arguments/processors/nsk_option_quiet.h"
#include "../arguments/processors/nsk_option_recursive.h"
#include "../arguments/processors/nsk_option_version.h"
#include "../types/pair/nsk_pair_type.h"
#include "../utils/nsk_util_size.h"

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
        "    op    ::= '=' | '!=' | '<>' | <' | '>' | '<=' | '>='\n"
        "    key   ::= [A-Za-z0-9_]+\n"
        "    value ::= uint64 (decimal)\n"
        "\n"
        "Semantics\n"
        "\n"
        "    * \"=\"  — exact equality\n"
        "    * \"!=\" — not equal\n"
        "    * \"<>\" - not equal\n"
        "    * \"<\"  — strictly less than\n"
        "    * \"<=\" — less or equal\n"
        "    * \">\"  — strictly greater than\n"
        "    * \">=\" — greater or equal\n"
        "    * No spaces; `&` separates conditions.\n"
        "\n"
        "Supported keys\n"
        "\n"
        "         key             values(*)        description\n"
        "    isnes20               0..1      iNES or NES 2.0 header format\n"
        "    console               0..12     Console type: NES, VS System, etc\n"
        "    region                0..3      Console region/timing (NTSC, PAL, etc)\n"
        "    mirroring             0..1      Vertical or Horizontal mirroring\n"
        "    alternative           0..1      Normal or alternative layout\n"
        "    mapper                0..4095   Mapper ID\n"
        "    submapper             0..15     Submapper ID\n"
        "    prg_rom               uint64    PRG ROM size\n"
        "    prg_ram               uint64    PRG RAM size\n"
        "    prg_nvram             uint64    PRG NVRAM size\n"
        "    chr_rom               uint64    CHR ROM size\n"
        "    chr_ram               uint64    CHR RAM size\n"
        "    chr_nvram             uint64    CHR NVRAM size\n"
        "    misc_roms             0..3      Miscellaneous ROMs count\n"
        "    battery               0..1      Battery NVRAM presence flag\n"
        "    trainer               0..1      Trainer area presence flag\n"
        "    device                0..78     Default expansion device\n"
        "    vs_ppu                0..11     Vs. System PPU type\n"
        "    vs_hardware           0..6      Vs. System hardware type\n"
        "\n"
        "(*) Any valid uint64_t value could be passed, but only listed values make \n"
        "    any sense.\n"
        "\n"
        "Examples\n"
        "\n"
        "    * \"mapper=305&prg_ram>0\"\n"
        "        Mapper ID is exactly 305 and PRG RAM size is > 0.\n"
        "\n"
        "    * \"region=1&chr_rom=0\"\n"
        "        PAL region/timings and no CHR ROM.\n"
        "\n"
        "    * \"prg_rom>262144&battery>0\"\n"
        "        PRG ROM larger than 256 KiB and battery-backed NVRAM present.\n"
        "\n"
        "    * \"isnes20=1&submapper>0\"\n"
        "        iNES 2.0 with a nonzero submapper.\n"
        "\n"
    },
    {
        "follow", 'l', no_argument,
        nsk_option_follow,
        "Follow symbolic links.\n"
        "By default no symbolic links are followed\n"
        "\n"
    },
    {
        "help", 'h', no_argument,
        nsk_option_help,
        "Print help and exit\n"
        "\n"
    },
    {
        "keys", 'k', required_argument,
        nsk_option_keys,
        "Sets the subset of keys to be shown. By default all keys are shown instead\n"
        "The string is a sequence of keys joined by `&`.\n"
        "\n"
        "Grammar\n"
        "\n"
        "    expr  ::= key ('&' key)*\n"
        "    key   ::= [A-Za-z0-9_]+\n"
        "\n"
        "Supported keys\n"
        "\n"
        "    See \"filter\" option description\n"
        "\n"
        "Exmaples\n"
        "\n"
        "    * \"mapper&prg_rom\"\n"
        "        Shows only mapper and PRG ROM fields\n"
        "\n"
    },
    {
        "output", 'o', required_argument,
        nsk_option_output,
        "Select the output type\n"
        "\n"
        "Possible values:\n"
        "    * \"tree\":\n"
        "        This is default output type. Shows the ROMs information as\n"
        "        detailed human-readable tree-like output\n"
        "\n"
        "    * \"table\":\n"
        "        Prints the ROMs information as a table\n"
        "        This output is compatible with Markdown tables format\n"
        "\n"
        "    * \"json\":\n"
        "        Prints the ROMs information as a JSON document\n"
        "        Uses the fields naming convention from the \"filter\" option description\n"
        "        Additionally enriches them with the \"filename\" field.\n"
        "        Every field (except \"filename\") is represented as an object, containing\n"
        "        \"raw\" and \"display\" values."
        "\n"
        "    * \"xml\":\n"
        "        Prints the ROMs information as an XML document\n"
        "        This format uses the notation suggested by NewRisingSun.\n"
        "        See https://forums.nesdev.org/viewtopic.php?f=3&t=19940&p=248796\n"
        "        for more information.\n"
        "        Quick ref: only present fields are printed, all but mirroring fields\n"
        "        are numeric. The mirroring is encoded as follows:\n"
        "        - Normal mappers:\n"
        "          |- encoding -|-  byte 6  -|\n"
        "          |     H      | .... .0..0 |\n"
        "          |     V      | .... .0..1 |\n"
        "          |     4      | .... .1..0 |\n"
        "\n"
        "        - Mapper 030:\n"
        "          |- encoding -|-  byte 6  -|\n"
        "          |     H      | .... .0..0 |\n"
        "          |     V      | .... .0..1 |\n"
        "          |     1      | .... .1..0 |\n"
        "          |     4      | .... .1..1 |\n"
        "\n"
        "        - Mapper 218:\n"
        "          |- encoding -|-  byte 6  -|\n"
        "          |     H      | .... .0..0 |\n"
        "          |     V      | .... .0..1 |\n"
        "          |     0      | .... .1..0 |\n"
        "          |     1      | .... .1..1 |\n"
        "\n"
        "        Differences from NewRisingSun's format:\n"
        "        - New field \"name\": ROM filename\n"
        "        - New field \"format\": attribute \"isnes20\"\n"
        "\n"
        "        Note: this output ignores the \"keys\" settings\n"
        "\n"
        "   * \"yaml\":\n"
        "        Prints the ROMs information as a YAML document\n"
        "        Uses the fields naming convention from the \"filter\" option description\n"
        "        Additionally enriches them with the \"filename\" field.\n"
        "        Every field (except \"filename\") is represented as an object, containing\n"
        "        \"raw\" and \"display\" values."
        "\n"
        "   * \"md\":\n"
        "        Prints the ROMs information as pretty Markdown document\n"
        "\n"
        "   * \"csv\":\n"
        "        Prints the ROMs information as a comma-separated CSV document\n"
        "\n"

    },
    {
        "quiet", 'q', no_argument,
        nsk_option_quiet,
        "Suppress any output but errors\n"
        "\n"
    },
    {
        "recursive", 'r', no_argument,
        nsk_option_recursive,
        "Enable recursive directory scanning.\n"
        "When specified, the program will process not only the given file(s)\n"
        "but also all subdirectories.\n"
        "\n"
    },
    {
        "version", 'v', no_argument,
        nsk_option_version,
        "Print program version and exit\n"
        "\n"
    },
};

/*!
 * Available options table size
 */
size_t nsk_options_count = NSK_SIZE(nsk_options_table);

/*!
 * \brief  Provided program options
 */
struct nsk_options_program nsk_options_program = { 0 };

/*!
 * \brief  Module finalizer
 */
__attribute__((destructor))
static void _fini(void) {
    nsk_pair_free(nsk_options_program.filter);
    nsk_pair_free(nsk_options_program.keys);
}
