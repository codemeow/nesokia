#ifndef NSK_HEADER_RAW
#define NSK_HEADER_RAW

#include <stdint.h>

/*!
 * @brief Represents the NES 2.0 ROM file header raw values.
 *
 * This structure defines the layout of the NES 2.0 header, which extends the
 * original iNES format with additional fields to describe more detailed
 * cartridge information, such as PRG/CHR sizes, submappers, timing modes, and
 * console types.
 */
struct __attribute__((__packed__)) nsk_header_raw {
    uint8_t magic[4]; /*!< File identification signature. */

    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t multiplier     : 2; /*!< Multiplier value */
            uint8_t exponent       : 6; /*!< Exponent value */
        };
    } prg_rom_lsb; /*!< Size of PRG ROM in 16 KB units (LSB). */

    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t multiplier     : 2; /*!< Multiplier value */
            uint8_t exponent       : 6; /*!< Exponent value */
        };
    } chr_rom_lsb; /*!< Size of CHR ROM in 8 KB units (LSB) */

    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t mirror         : 1; /*!< Mirroring type */
            uint8_t battery_nvram  : 1; /*!< Battery NVRAM presence flag */
            uint8_t trainer        : 1; /*!< Trainer section presence flag */
            uint8_t alt_nametables : 1; /*!< Alternative nametables flag */
            uint8_t mapper_low     : 4; /*!< Lower bits of the mapper index (0 00 XX) */
        };
    } flags6; /*!< Basic cartridge configuration. */

    /*!
     * @brief Flags 7:
     */
    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t console_type   : 2; /*!< Console type */
            uint8_t nes20_magic    : 2; /*!< NES 2.0 format signature (=0b10) */
            uint8_t mapper_mid     : 4; /*!< Middle bits of the mapper index (0 XX 00) */
        };
    } flags7; /*!< Console type and mapper continuation. */

    /*!
     * @brief Mappers info
     */
    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t mapper_high    : 4; /*!< High bits of the mapper index (X 00 00) */
            uint8_t submapper      : 4; /*!< Submapper ID */
        };
    } mappers;

    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t prg_msb        : 4; /*!< Size of PRG ROM in 16 KB units (MSB) */
            uint8_t chr_msb        : 4; /*!< Size of CHR ROM in 8 KB units (MSB) */
        };
    } roms_msb;

    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t ram_shift      : 4; /*!< Size of PRG RAM in 64 << shifts */
            uint8_t nvram_shift    : 4; /*!< Size of PRG NVRAM in 64 << shifts */
        };
    } prg_ram;

    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t ram_shift      : 4; /*!< Size of CHR RAM in 64 << shifts */
            uint8_t nvram_shift    : 4; /*!< Size of CHR NVRAM in 64 << shifts */
        };
    } chr_ram;

    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t timing_mode    : 2; /*!< Timing/region type */
            uint8_t _reserved      : 6; /*!< Not used */
        };
    } timing;

    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t ppu            : 4; /*!< PPU type */
            uint8_t hardware       : 4; /*!< Hardware type */
        } vssystem; /*!< Converted value (if the flags7.console_type = 1) */

        struct {
            uint8_t console_subtype : 4; /*!< Console subtype */
            uint8_t _reserved       : 4; /*!< Not used */
        } extended; /*!< Converted value (if the flags7.console_type = 3) */
    } subtype;

    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t rom_count       : 2; /*!< Misc ROMs count */
            uint8_t _reserved       : 6; /*!< Not used */
        };
    } misc;

    union {
        uint8_t raw; /*!< Raw byte value */

        struct {
            uint8_t device          : 6; /*!< Default expansion device */
            uint8_t _reserved       : 2; /*!< Not used */
        };
    } expansion;
};

#endif