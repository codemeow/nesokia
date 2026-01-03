#include <string.h>
#include <nsk_util_meta.h>

#include "../../types/entry/nsk_entry_read.h"

/*!
 * \brief  Validates the read header over iNES/NES2.0 signature
 *
 * \param[in]  header  The header
 * \return True if the header was read fro iNES/NES2.0 file
 */
static bool _rom_validate(const struct nsk_header_raw *header) {
    static const uint8_t NESmagic[] = { 'N', 'E', 'S', 0x1a };

    return memcmp(header->magic, NESmagic, sizeof(NESmagic)) == 0;
}

/*!
 * \brief  Read the header from the ROM file
 *
 * \param[in] filename  The filename
 * \param[out] header    The header
 * \return True on success
 */
bool nsk_entry_read(const char *filename, struct nsk_header_raw *header) {
    nsk_auto_fclose FILE *file = nsk_io_fopen(filename, "rb");
    if (!file) {
        nsk_err(
            "Error: cannot open file \"%s\"\n",
            filename
        );
        return false;
    }

    size_t res = fread(header, sizeof(*header), 1, file);
    if (res != 1) {
        nsk_err(
            "Error: cannot read file \"%s\"\n", filename
        );
        return false;
    }

    if (!_rom_validate(header)) {
        nsk_err(
            "Error: \"%s\" is not a iNES/NES2.0 file\n", filename
        );
        return false;
    }

    return true;
}