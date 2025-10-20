#include "../ROM/nsk_rom_process.h"
#include "../ROM/header/nsk_header_raw.h"
#include "../ROM/header/nsk_header_data.h"
#include "../ROM/header/nsk_header_show.h"
#include "../ROM/nsk_rom_read.h"
#include "../arguments/nsk_args_options.h"

/*!
 * \brief  Process the possible ROM file
 *
 * \param[in] filename  The filename
 */
void nsk_rom_process(const char *filename) {
    struct nsk_header_raw raw;
    struct nsk_header_data data;

    if (!nsk_rom_read(filename, &raw)) {
        return;
    }

    data = nsk_header_raw2data(&raw);

//    if (nsk_filter_match(&data)) {
        if (nsk_options_program.table) {
            //nsk_table_push(filename, &data);

        } else {
            nsk_header_show(filename, &data);
        }
//    }
}