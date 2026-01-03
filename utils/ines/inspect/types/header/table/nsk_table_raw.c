#include <stdint.h>
#include <nsk_util_meta.h>

#include "../../../types/header/table/nsk_table_raw.h"

/*!
 * \brief  Converts NES 2.0 signature
 *
 * \param[in] raw   The raw
 * \param     data  The data */
void nsk_convraw_isnes20(
    const struct nsk_header_raw *raw,
    struct nsk_header_data *data
) {
    data->isNES20 = raw->flags7.nes20_magic == nsk_magic_NES20;
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

    if (data->isNES20) {
        data->ROM.prg = nsk_util_expsize(
            raw->NES20.roms_msb.prg_msb,
            raw->prg_rom_lsb.raw,
            multiplier
        );

    } else {
        data->ROM.prg = nsk_util_expsize(
            0,
            raw->prg_rom_lsb.raw,
            multiplier
        );
    }
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

    if (data->isNES20) {
        data->ROM.chr = nsk_util_expsize(
            raw->NES20.roms_msb.chr_msb,
            raw->chr_rom_lsb.raw,
            multiplier
        );

    } else {
        data->ROM.chr = nsk_util_expsize(
            0,
            raw->chr_rom_lsb.raw,
            multiplier
        );
    }
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
    if (data->isNES20) {
        data->ROM.misc = raw->NES20.misc.rom_count;

    } else {
        data->ROM.misc = 0;
    }
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
    if (data->isNES20) {
        data->RAM.prg =
            raw->NES20.prg_ram.ram_shift > 0
            ? 64 << raw->NES20.prg_ram.ram_shift
            : 0;

    } else {
        const uint32_t multiplier = 8 * 1024;

        data->RAM.prg =
            raw->iNES.prg_ram == 0
            ? multiplier
            : multiplier * raw->iNES.prg_ram;
    }
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
    if (data->isNES20) {
        data->RAM.chr =
            raw->NES20.chr_ram.ram_shift > 0
            ? 64 << raw->NES20.chr_ram.ram_shift
            : 0;

    } else {
        data->RAM.chr = 0;
    }
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
    if (data->isNES20) {
        data->NVRAM.prg =
            raw->NES20.prg_ram.nvram_shift > 0
            ? 64 << raw->NES20.prg_ram.nvram_shift
            : 0;

    } else {
        data->NVRAM.prg = 0;
    }
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
    if (data->isNES20) {
        data->NVRAM.chr =
            raw->NES20.chr_ram.nvram_shift > 0
            ? 64 << raw->NES20.chr_ram.nvram_shift
            : 0;

    } else {
        data->NVRAM.chr = 0;
    }
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
    if (data->isNES20) {
        data->mapper.id =
            raw->NES20.mappers.mapper_high << 8 |
            raw->flags7.mapper_mid         << 4 |
            raw->flags6.mapper_low;

    } else {
        data->mapper.id =
            raw->flags7.mapper_mid         << 4 |
            raw->flags6.mapper_low;
    }
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
    if (data->isNES20) {
        data->mapper.subid = raw->NES20.mappers.submapper;

    } else {
        data->mapper.subid = 0;
    }
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
    data->nametables.mirror = raw->flags6.mirror;
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
    if (data->isNES20) {
        if (raw->flags7.console_type > NSK_CONSOLE_PLAYCHOICE10) {
            data->hardware.console.type =
                raw->NES20.subtype.extended.console_subtype;

        } else {
            data->hardware.console.type =
                raw->flags7.console_type;
        }

    } else {
        switch (raw->flags7.console_type) {
            case 0:
                data->hardware.console.type = NSK_CONSOLE_NES;
                break;
            case 1:
                data->hardware.console.type = NSK_CONSOLE_NINTENDO_VS;
                break;

            default:
                /* Despite the 2nd bit may signal of Playchoice-10 it is
                 * still not a part of the official specification.
                 * Considering anything else as garbage */
                data->hardware.console.type = NSK_CONSOLE_NES;
        }
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
    if (data->isNES20) {
        data->hardware.console.region = raw->NES20.timing.timing_mode;

    } else {
        data->hardware.console.region = raw->iNES.timing.timing_mode;
    }
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
    if (data->isNES20) {
        data->hardware.expansion = raw->NES20.expansion.device;

    } else {
        data->hardware.expansion = NSK_DEVICE_NONE_UNSPECIFIED;
    }
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
    if (data->isNES20) {
        if (raw->flags7.console_type == NSK_CONSOLE_NINTENDO_VS) {
            data->hardware.console.vs.ppu =
                raw->NES20.subtype.vssystem.ppu;

        } else {
            data->hardware.console.vs.ppu = NSK_VSPPU_NONE;
        }

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
    if (data->isNES20) {
        if (raw->flags7.console_type == NSK_CONSOLE_NINTENDO_VS) {
            data->hardware.console.vs.hardware =
                raw->NES20.subtype.vssystem.hardware;

        } else {
            data->hardware.console.vs.hardware = NSK_VSHARDWARE_NONE;
        }

    } else {
        data->hardware.console.vs.hardware = NSK_VSHARDWARE_NONE;
    }
}
