#include <stdint.h>

#include "../../../ROM/header/table/nsk_table_raw.h"
#include "../../../utils/nsk_util_expsize.h"

/*!
 * \brief  Converts NES 2.0 signature
 *
 * \param[in] raw   The raw
 * \param     data  The data */
void nsk_convraw_isnes20(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    static const uint8_t NES20_magic = 0x02;
    data->isNES20 = raw->flags7.nes20_magic == NES20_magic;
}

/*!
 * \brief  Converts PRG ROM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_prgrom(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    const uint32_t multiplier = 16 * 1024;

    data->ROM.prg = nsk_util_expsize(
        raw->roms_msb.prg_msb,
        raw->prg_rom_lsb.raw,
        multiplier
    );
}

/*!
 * \brief  Converts CHR ROM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_chrrom(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    const uint32_t multiplier = 8 * 1024;

    data->ROM.prg = nsk_util_expsize(
        raw->roms_msb.chr_msb,
        raw->chr_rom_lsb.raw,
        multiplier
    );
}

/*!
 * \brief  Converts Miscellaneous ROMs
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_miscroms(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->ROM.misc = raw->misc.rom_count;
}

/*!
 * \brief  Converts PRG RAM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_prgram(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->RAM.prg =
        raw->prg_ram.ram_shift > 0
        ? 64 << raw->prg_ram.ram_shift
        : 0;
}

/*!
 * \brief  Converts CHR RAM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_chrram(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->RAM.chr =
        raw->chr_ram.ram_shift > 0
        ? 64 << raw->chr_ram.ram_shift
        : 0;
}

/*!
 * \brief  Converts PRG NVRAM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_prgnvram(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->RAM.prg =
        raw->prg_ram.nvram_shift > 0
        ? 64 << raw->prg_ram.nvram_shift
        : 0;
}

/*!
 * \brief  Converts CHR NVRAM
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_chrnvram(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->RAM.chr =
        raw->chr_ram.nvram_shift > 0
        ? 64 << raw->chr_ram.nvram_shift
        : 0;
}

/*!
 * \brief  Converts mapper id
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_mapper(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->mapper.id =
        raw->mappers.mapper_high << 8 |
        raw->flags7.mapper_mid   << 4 |
        raw->flags6.mapper_low;
}

/*!
 * \brief  Converts submapper id
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_submapper(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->mapper.subid = raw->mappers.submapper;
}

/*!
 * \brief  Converts nametable's layout
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_mirroring(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->nametables.isvertical = raw->flags6.mirror;
}

/*!
 * \brief  Converts alternative nametables flag
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_alternative(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->nametables.alternative = raw->flags6.alt_nametables;
}

/*!
 * \brief  Converts the trainer
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_trainer(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->trainer = raw->flags6.trainer;
}

/*!
 * \brief  Converts battery
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_battery(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->battery = raw->flags6.battery_nvram;
}

/*!
 * \brief  Converts console type
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_console(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    if (raw->flags7.console_type > NSK_CONSOLE_PLAYCHOICE10) {
        data->hardware.console.type = raw->subtype.extended.console_subtype;
    } else {
        data->hardware.console.type = raw->flags7.console_type;
    }
}

/*!
 * \brief  Converts region/timing
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_region(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->hardware.console.region = raw->timing.timing_mode;
}

/*!
 * \brief  Converts expansion device type
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_device(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->hardware.expansion = raw->expansion.device;
}

/*!
 * \brief  Converts Vs. PPU type
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_vsppu(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    if (raw->flags7.console_type == NSK_CONSOLE_NINTENDO_VS) {
        data->hardware.console.vs.ppu = raw->subtype.vssystem.ppu;
    } else {
        data->hardware.console.vs.ppu = NSK_VSPPU_NONE;
    }
}

/*!
 * \brief  Converts Vs. hardware type
 *
 * \param[in]  raw   The raw header
 * \param[out] data  Converted data
 */
void nsk_convraw_vshardware(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    if (raw->flags7.console_type == NSK_CONSOLE_NINTENDO_VS) {
        data->hardware.console.vs.hardware = raw->subtype.vssystem.hardware;
    } else {
        data->hardware.console.vs.hardware = NSK_VSHARDWARE_NONE;
    }
}