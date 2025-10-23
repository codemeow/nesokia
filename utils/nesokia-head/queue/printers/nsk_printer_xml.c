#include <inttypes.h>
#include <stdlib.h>

#include "../../queue/printers/nsk_printer_xml.h"

#include "../../utils/log/nsk_log_inf.h"
#include "../../types/entry/nsk_entry_storage.h"
#include "../../types/entry/nsk_entry_type.h"
#include "../../types/header/nsk_header_data.h"
#include "../../utils/nsk_util_size.h"
#include "../../utils/log/nsk_log_err.h"

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
                [data->nametables.mirror == NSK_MIRRORING_HORIZONTAL]
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
        const char *name;
        uint64_t    value;
    } table_simple_u64[] = {
        { "prgrom", data->ROM.prg },
        { "chrrom", data->ROM.chr },
        { "prgram", data->RAM.prg },
        { "chrram", data->RAM.chr },
        { "prgnvram", data->NVRAM.prg },
        { "chrnvram", data->NVRAM.chr },
    };


    for (size_t i = 0; i < NSK_SIZE(table_simple_u64); i++) {
        if (table_simple_u64[i].value > 0) {
            nsk_inf(
                "\t\t<%s size=\"%" PRIu64 "\" />\n",
                table_simple_u64[i].name,
                table_simple_u64[i].value
            );
        }
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
    nsk_inf(
        "\t\t<pcb "
            "mapper=\"%" PRIu16 "\" "
            "submapper=\"%" PRIu8 "\" "
            "mirroring=\"%c\" "
            "battery=\"%d\" "
        "/>\n",
        data->mapper.id,
        data->mapper.subid,
        _print_mirroring(data),
        data->battery
    );
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
    if (data->trainer) {
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
    nsk_inf(
        "\t\t<console type=\"%d\" region=\"%d\" />\n",
        data->hardware.console.type,
        data->hardware.console.region
    );
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
    if (data->hardware.console.type == NSK_CONSOLE_NINTENDO_VS) {
        nsk_inf(
            "\t\t<vs hardware=\"%d\" ppu=\"%d\" />\n",
            data->hardware.console.vs.hardware,
            data->hardware.console.vs.ppu
        );
    }
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
    if (data->hardware.expansion != NSK_DEVICE_NONE_UNSPECIFIED) {
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