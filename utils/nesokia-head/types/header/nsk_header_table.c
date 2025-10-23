#include <stdlib.h>

#include "../../types/header/nsk_header_table.h"

#include "../../types/header/nsk_header_data.h"
#include "../../types/header/table/nsk_table_cmp.h"
#include "../../types/header/table/nsk_table_long.h"
#include "../../types/header/table/nsk_table_raw.h"
#include "../../types/header/table/nsk_table_short.h"
#include "../../utils/nsk_util_size.h"

/*!
 * \brief  Shortcut offsetof-macro
 */
#define NSK_OF(field) offsetof(struct nsk_header_data, field)

/*!
 * The global header data table for the processors syncing
 */
const struct nsk_header_tableentry nsk_header_table[] = {
    /* Format */
    {
        .category   = NSK_CATEGORY_FORMAT,
        .description= "Is NES 2.0?",
        .shortcut   = "isnes20",
        .length     = 1,
        .shift      = NSK_OF(isNES20),
        .conv_raw   = nsk_convraw_isnes20,
        .conv_long  = nsk_convlong_bool,
        .conv_short = nsk_convshort_bool,
        .match      = nsk_compare_bool
    },

    /* Console */
    {
        .category   = NSK_CATEGORY_CONSOLE,
        .description= "Type",
        .shortcut   = "console",
        .length     = 2,
        .shift      = NSK_OF(hardware.console.type),
        .conv_raw   = nsk_convraw_console,
        .conv_long  = nsk_convlong_type,
        .conv_short = nsk_convshort_type,
        .match      = nsk_compare_type
    },
    {
        .category   = NSK_CATEGORY_CONSOLE,
        .description= "Timings / region",
        .shortcut   = "region",
        .length     = 2,
        .shift      = NSK_OF(hardware.console.region),
        .conv_raw   = nsk_convraw_region,
        .conv_long  = nsk_convlong_region,
        .conv_short = nsk_convshort_region,
        .match      = nsk_compare_region
    },
    {
        .category   = NSK_CATEGORY_CONSOLE,
        .description= "Vs. PPU type",
        .shortcut   = "vs_ppu",
        .length     = 2,
        .shift      = NSK_OF(hardware.console.vs.ppu),
        .conv_raw   = nsk_convraw_vsppu,
        .conv_long  = nsk_convlong_vsppu,
        .conv_short = nsk_convshort_vsppu,
        .match      = nsk_compare_vsppu
    },
    {
        .category   = NSK_CATEGORY_CONSOLE,
        .description= "Vs. Hardware type",
        .shortcut   = "vs_hardware",
        .length     = 1,
        .shift      = NSK_OF(hardware.console.vs.hardware),
        .conv_raw   = nsk_convraw_vshardware,
        .conv_long  = nsk_convlong_vshardware,
        .conv_short = nsk_convshort_vshardware,
        .match      = nsk_compare_vshardware
    },

    /* Nametables */
    {
        .category   = NSK_CATEGORY_NAMETABLES,
        .description= "Mirroring / layout",
        .shortcut   = "mirroring",
        .length     = 1,
        .shift      = NSK_OF(nametables.mirror),
        .conv_raw   = nsk_convraw_mirroring,
        .conv_long  = nsk_convlong_mirroring,
        .conv_short = nsk_convshort_mirroring,
        .match      = nsk_compare_mirroring
    },
    {
        .category   = NSK_CATEGORY_NAMETABLES,
        .description= "Alternative layout",
        .shortcut   = "alternative",
        .length     = 1,
        .shift      = NSK_OF(nametables.alternative),
        .conv_raw   = nsk_convraw_alternative,
        .conv_long  = nsk_convlong_bool,
        .conv_short = nsk_convshort_bool,
        .match      = nsk_compare_bool
    },

    /* Mapper */
    {
        .category   = NSK_CATEGORY_MAPPER,
        .description= "Mapper ID",
        .shortcut   = "mapper",
        .length     = 4,
        .shift      = NSK_OF(mapper.id),
        .conv_raw   = nsk_convraw_mapper,
        .conv_long  = nsk_convlong_u16,
        .conv_short = nsk_convshort_u16,
        .match      = nsk_compare_u16
    },
    {
        .category   = NSK_CATEGORY_MAPPER,
        .description= "Submapper ID",
        .shortcut   = "submapper",
        .length     = 1,
        .shift      = NSK_OF(mapper.subid),
        .conv_raw   = nsk_convraw_submapper,
        .conv_long  = nsk_convlong_u8,
        .conv_short = nsk_convshort_u8,
        .match      = nsk_compare_u8
    },

    /* Memory */
    {
        .category   = NSK_CATEGORY_MEMORY,
        .description= "PRG ROM",
        .shortcut   = "prg_rom",
        .length     = 10,
        .shift      = NSK_OF(ROM.prg),
        .conv_raw   = nsk_convraw_prgrom,
        .conv_long  = nsk_convlong_bytes,
        .conv_short = nsk_convshort_u64,
        .match      = nsk_compare_u64
    },
    {
        .category   = NSK_CATEGORY_MEMORY,
        .description= "CHR ROM",
        .shortcut   = "chr_rom",
        .length     = 10,
        .shift      = NSK_OF(ROM.chr),
        .conv_raw   = nsk_convraw_chrrom,
        .conv_long  = nsk_convlong_bytes,
        .conv_short = nsk_convshort_u64,
        .match      = nsk_compare_u64
    },
    {
        .category   = NSK_CATEGORY_MEMORY,
        .description= "PRG RAM",
        .shortcut   = "prg_ram",
        .length     = 10,
        .shift      = NSK_OF(RAM.prg),
        .conv_raw   = nsk_convraw_prgram,
        .conv_long  = nsk_convlong_bytes,
        .conv_short = nsk_convshort_u64,
        .match      = nsk_compare_u64
    },
    {
        .category   = NSK_CATEGORY_MEMORY,
        .description= "CHR RAM",
        .shortcut   = "chr_ram",
        .length     = 10,
        .shift      = NSK_OF(RAM.chr),
        .conv_raw   = nsk_convraw_chrram,
        .conv_long  = nsk_convlong_bytes,
        .conv_short = nsk_convshort_u64,
        .match      = nsk_compare_u64
    },
    {
        .category   = NSK_CATEGORY_MEMORY,
        .description= "PRG NVRAM",
        .shortcut   = "prg_nvram",
        .length     = 10,
        .shift      = NSK_OF(NVRAM.prg),
        .conv_raw   = nsk_convraw_prgnvram,
        .conv_long  = nsk_convlong_bytes,
        .conv_short = nsk_convshort_u64,
        .match      = nsk_compare_u64
    },
    {
        .category   = NSK_CATEGORY_MEMORY,
        .description= "CHR NVRAM",
        .shortcut   = "chr_nvram",
        .length     = 10,
        .shift      = NSK_OF(NVRAM.chr),
        .conv_raw   = nsk_convraw_chrnvram,
        .conv_long  = nsk_convlong_bytes,
        .conv_short = nsk_convshort_u64,
        .match      = nsk_compare_u64
    },
    {
        .category   = NSK_CATEGORY_MEMORY,
        .description= "Miscellaneous ROMs",
        .shortcut   = "misc_roms",
        .length     = 1,
        .shift      = NSK_OF(ROM.misc),
        .conv_raw   = nsk_convraw_miscroms,
        .conv_long  = nsk_convlong_u8,
        .conv_short = nsk_convshort_u8,
        .match      = nsk_compare_u8
    },

    /* Miscellaneous info */
    {
        .category   = NSK_CATEGORY_MISC,
        .description= "Trainer area",
        .shortcut   = "trainer",
        .length     = 1,
        .shift      = NSK_OF(trainer),
        .conv_raw   = nsk_convraw_trainer,
        .conv_long  = nsk_convlong_bool,
        .conv_short = nsk_convshort_bool,
        .match      = nsk_compare_bool
    },
    {
        .category   = NSK_CATEGORY_MISC,
        .description= "Non volatile memory",
        .shortcut   = "battery",
        .length     = 1,
        .shift      = NSK_OF(battery),
        .conv_raw   = nsk_convraw_battery,
        .conv_long  = nsk_convlong_bool,
        .conv_short = nsk_convshort_bool,
        .match      = nsk_compare_bool
    },
    {
        .category   = NSK_CATEGORY_MISC,
        .description= "Expansion device",
        .shortcut   = "device",
        .length     = 2,
        .shift      = NSK_OF(hardware.expansion),
        .conv_raw   = nsk_convraw_device,
        .conv_long  = nsk_convlong_device,
        .conv_short = nsk_convshort_device,
        .match      = nsk_compare_device
    },
};

/*!
 * The size of the global header table;
 */
const size_t nsk_header_tablesize = NSK_SIZE(nsk_header_table);
