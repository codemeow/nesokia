#include <inttypes.h>
#include <stdlib.h>
#include <nsk_util_meta.h>

#include "../../queue/printers/nsk_printer_xml.h"
#include "../../queue/printers/nsk_printer_match.h"
#include "../../types/entry/nsk_entry_storage.h"
#include "../../types/entry/nsk_entry_type.h"
#include "../../types/header/nsk_header_data.h"

/*!
 * \brief  Prints the XML header
 */
static void _print_header(void) {
    nsk_inf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    nsk_inf("<scan>\n");
}

/*!
 * \brief  Prints the XML footer
 */
static void _print_footer(void) {
    nsk_inf("</scan>\n");
}

/*!
 * \brief  Mapper 30 checker
 *
 * See https://www.nesdev.org/wiki/UNROM_512#Nametable_Configuration
 *
 * \param[in] mapper  The mapper
 * \return True if the specified mapper is mapper 30, False otherwise.
 */
static bool _is_mapper30(uint16_t mapper) {
    return mapper == 30;
}

/*!
 * \brief  Mapper 218 checker
 *
 * \param[in] mapper  The mapper
 * \return    True if the specified mapper is mapper 218, False otherwise.
 */
static bool _is_mapper218(uint16_t mapper) {
    return mapper == 218;
}

/*!
 * \brief  Any mapper
 *
 * \param[in] mapper  The mapper
 * \return Always true
 */
static bool _is_mapperany(uint16_t mapper __attribute__((unused))) {
    return true;
}

static char _print_mirroring(const struct nsk_header_data *data) {
    static const struct {
        bool (*selector)(uint16_t mapper);
        /* [mirror = HOR][alt] */
        const char values[2][2];
    } table[] = {
        { _is_mapper30,  { {'1', 'H'}, {'4', 'V'} } },
        { _is_mapper218, { {'0', 'H'}, {'1', 'V'} } },
        { _is_mapperany, { {'4', 'H'}, {'V', 'V'} } }
    };

    for (size_t i = 0; i < NSK_SIZE(table); i++) {
        if (table[i].selector(data->mapper.id)) {
            return table[i].values
                [data->nametables.mirror == NSK_MIRRORING_HORIZONTAL_MIRRORING]
                [data->nametables.alternative];
        }
    }

    nsk_err(
        "Error: no mapper selector applied for %u" PRIu16 "\n",
        data->mapper.id
    );
    abort();
}

/*!
 * \brief  Prints game header data
 *
 * \param[in] filename  The filename
 * \param[in] data      The data
 */
static void _print_game_header(
    const char *filename,
    const struct nsk_header_data *data __attribute__((unused))
) {
    nsk_inf("\t<game><!-- %s -->\n", filename);
    nsk_inf("\t\t<name>%s</name>\n", filename);
}

/*!
 * \brief  Prints game format data
 *
 * \param[in] filename  The filename
 * \param[in] data      The data
 */
static void _print_game_format(
    const char *filename __attribute__((unused)),
    const struct nsk_header_data *data
) {
    if (!nsk_printer_match("isnes20")) {
        return;
    }

    nsk_inf("\t\t<format isnes20=\"%d\" />\n", data->isNES20);
}

/*!
 * \brief  Prints game memory data
 *
 * \param[in] filename  The filename
 * \param[in] data      The data
 */
static void _print_game_memory(
    const char *filename __attribute__((unused)),
    const struct nsk_header_data *data
) {
    struct {
        const char *shortcut;
        const char *tag;
        uint64_t    value;
    } table_simple_u64[] = {
        { "prg_rom",   "prgrom",   data->ROM.prg    },
        { "chr_rom",   "chrrom",   data->ROM.chr    },
        { "prg_ram",   "prgram",   data->RAM.prg    },
        { "chr_ram",   "chrram",   data->RAM.chr    },
        { "prg_nvram", "prgnvram", data->NVRAM.prg  },
        { "chr_nvram", "chrnvram", data->NVRAM.chr  },
    };


    for (size_t i = 0; i < NSK_SIZE(table_simple_u64); i++) {
        if (
            table_simple_u64[i].value > 0 &&
            nsk_printer_match(table_simple_u64[i].shortcut)
        ) {
            nsk_inf(
                "\t\t<%s size=\"%" PRIu64 "\" />\n",
                table_simple_u64[i].tag,
                table_simple_u64[i].value
            );
        }
    }

    if (data->ROM.misc > 0 && nsk_printer_match("misc_roms")) {
        nsk_inf("\t\t<miscrom count=\"%u\" />\n", data->ROM.misc);
    }
}

/*!
 * \brief  Prints game hardware data
 *
 * \param[in] filename  The filename
 * \param[in] data      The data
 */
static void _print_game_pcb(
    const char *filename __attribute__((unused)),
    const struct nsk_header_data *data
) {
    bool print_mapper      = nsk_printer_match("mapper");
    bool print_submapper   = nsk_printer_match("submapper");
    bool print_mirroring   = nsk_printer_match("mirroring");
    bool print_alternative = nsk_printer_match("alternative");
    bool print_battery     = nsk_printer_match("battery");

    if (
        !print_mapper &&
        !print_submapper &&
        !print_mirroring &&
        !print_alternative &&
        !print_battery
    ) {
        return;
    }

    nsk_inf("\t\t<pcb");
    if (print_mapper) {
        nsk_inf(" mapper=\"%" PRIu16 "\"", data->mapper.id);
    }
    if (print_submapper) {
        nsk_inf(" submapper=\"%" PRIu8 "\"", data->mapper.subid);
    }
    if (print_mirroring || print_alternative) {
        nsk_inf(" mirroring=\"%c\"", _print_mirroring(data));
    }
    if (print_battery) {
        nsk_inf(" battery=\"%d\"", data->battery);
    }
    nsk_inf(" />\n");
}

/*!
 * \brief  Prints game trainer data
 *
 * \param[in] filename  The filename
 * \param[in] data      The data
 */
static void _print_game_trainer(
    const char *filename __attribute__((unused)),
    const struct nsk_header_data *data
) {
    if (data->trainer && nsk_printer_match("trainer")) {
        nsk_inf("\t\t<trainer size=\"512\" />\n");
    }
}

/*!
 * \brief  Prints game console data
 *
 * \param[in] filename  The filename
 * \param[in] data      The data
 */
static void _print_game_console(
    const char *filename __attribute__((unused)),
    const struct nsk_header_data *data
) {
    bool print_console = nsk_printer_match("console");
    bool print_region  = nsk_printer_match("region");

    if (!print_console && !print_region) {
        return;
    }

    nsk_inf("\t\t<console");
    if (print_console) {
        nsk_inf(" type=\"%d\"", data->hardware.console.type);
    }
    if (print_region) {
        nsk_inf(" region=\"%d\"", data->hardware.console.region);
    }
    nsk_inf(" />\n");
}

/*!
 * \brief  Prints game Vs. System data
 *
 * \param[in] filename  The filename
 * \param[in] data      The data
 */
static void _print_game_vs(
    const char *filename __attribute__((unused)),
    const struct nsk_header_data *data
) {
    bool print_hardware = nsk_printer_match("vs_hardware");
    bool print_ppu      = nsk_printer_match("vs_ppu");

    if (
        data->hardware.console.type != NSK_CONSOLE_NINTENDO_VS ||
        (!print_hardware && !print_ppu)
    ) {
        return;
    }

    nsk_inf("\t\t<vs");
    if (print_hardware) {
        nsk_inf(" hardware=\"%d\"", data->hardware.console.vs.hardware);
    }
    if (print_ppu) {
        nsk_inf(" ppu=\"%d\"", data->hardware.console.vs.ppu);
    }
    nsk_inf(" />\n");
}

/*!
 * \brief  Prints game expansion device data
 *
 * \param[in] filename  The filename
 * \param[in] data      The data
 */
static void _print_game_device(
    const char *filename __attribute__((unused)),
    const struct nsk_header_data *data
) {
    if (
        data->hardware.expansion != NSK_DEVICE_NONE_UNSPECIFIED &&
        nsk_printer_match("device")
    ) {
        nsk_inf(
            "\t\t<expansion type=\"%d\" />\n",
            data->hardware.expansion
        );
    }
}

/*!
 * \brief  Prints game footer data
 *
 * \param[in] filename  The filename
 * \param[in] data      The data
 */
static void _print_game_footer(
    const char *filename __attribute__((unused)),
    const struct nsk_header_data *data __attribute__((unused))
) {
    nsk_inf("\t</game>\n");
}

/*!
 * \brief  Prints one single entry
 *
 * \param[in] filename  The filename
 * \param[in] data      The data
 */
static void _print_entry(
    const char *filename,
    const struct nsk_header_data *data
) {
    static const struct {
        void (*printer)(
            const char *filename,
            const struct nsk_header_data *data
        );
    } table[] = {
        { _print_game_header    },
        { _print_game_format    },
        { _print_game_memory    },
        { _print_game_pcb       },
        { _print_game_trainer   },
        { _print_game_console   },
        { _print_game_vs        },
        { _print_game_device    },
        { _print_game_footer    },
    };

    for (size_t i = 0; i < NSK_SIZE(table); i++) {
        table[i].printer(filename, data);
    }
}

/*!
 * \brief  Prints all the entries
 */
static void _print_entries(void) {
    struct nsk_entry *entry = nsk_entry_storage;
    while (entry) {
        _print_entry(entry->filename, &entry->data);

        entry = nsk_entry_next(entry);
    }
}

/*!
 * \brief  Prints the ROMs information as an XML document
 */
void nsk_printer_xml(void) {
    _print_header();
    _print_entries();
    _print_footer();
}
