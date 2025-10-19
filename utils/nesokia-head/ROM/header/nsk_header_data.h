#ifndef NSK_HEADER_DATA
#define NSK_HEADER_DATA

#include <stdbool.h>
#include <stdint.h>

#include "../../ROM/header/nsk_header_raw.h"

#include "../../ROM/types/nsk_romtype_console.h"
#include "../../ROM/types/nsk_romtype_region.h"
#include "../../ROM/types/nsk_romtype_vsppu.h"
#include "../../ROM/types/nsk_romtype_vshardware.h"
#include "../../ROM/types/nsk_romtype_device.h"

/*!
 * \brief  Parsed and structured ROM header data
 *
 * \note This structure must not have any unions
 */
struct nsk_header_data {
    bool isNES20;           /*!< NES 2.0 signature                            */

    struct {
        uint64_t prg;       /*!< PRG ROM size in bytes                        */
        uint64_t chr;       /*!< CHR ROM size in bytes                        */
        uint8_t  misc;      /*!< Number of miscellaneous ROMs                 */
    } ROM;

    struct {
        uint64_t prg;       /*!< PRG RAM size in bytes                        */
        uint64_t chr;       /*!< CHR RAM size in bytes                        */
    } RAM;

    struct {
        uint64_t prg;       /*!< PRG NVRAM size in bytes                      */
        uint64_t chr;       /*!< CHR NVRAM size in bytes                      */
    } NVRAM;

    struct {
        uint16_t id;        /*!< Mapper ID                                    */
        uint8_t  subid;     /*!< Submapper ID                                 */
    } mapper;

    struct {
        bool isvertical;    /*!< Is Vertical alignment (or mapper-controlled) */
        bool alternative;   /*!< Is alternative nametables layout used        */
    } nametables;

    bool trainer;           /*!< Is trainer section present                   */
    bool battery;           /*!< Is battery backed NVROM is present           */

    struct {
        struct {
            enum nsk_console_type    type;        /*!< Console type           */
            enum nsk_region_type     region;      /*!< Console region         */

            struct {
                enum nsk_vsppu_type      ppu;     /*!< Vs system PPU          */
                enum nsk_vshardware_type hardware;/*!< Vs system hardware type*/
            } vs;
        } console;

        enum nsk_device_type expansion;    /*!< Default expansion device */
    } hardware;
};

/*!
 * \brief  Converts raw header to parsed data
 *
 * \param[in] raw  The raw header values
 * \return Parsed header data
 */
struct nsk_header_data nsk_header_raw2data(const struct nsk_header_raw *raw);

#endif