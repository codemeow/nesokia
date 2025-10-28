#ifndef NSK_HEADER_DATA
#define NSK_HEADER_DATA

#include <stdbool.h>
#include <stdint.h>

#include "../../types/header/enums/nsk_enum_alternative.h"
#include "../../types/header/enums/nsk_enum_battery.h"
#include "../../types/header/enums/nsk_enum_console.h"
#include "../../types/header/enums/nsk_enum_device.h"
#include "../../types/header/enums/nsk_enum_mirroring.h"
#include "../../types/header/enums/nsk_enum_region.h"
#include "../../types/header/enums/nsk_enum_trainer.h"
#include "../../types/header/enums/nsk_enum_vshardware.h"
#include "../../types/header/enums/nsk_enum_vsppu.h"

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
        enum nsk_mirroring_type mirror; /*!< Mirroring mode                   */
        enum nsk_alternative_type alternative;   /*!< Alternative layout      */
    } nametables;

    enum nsk_trainer_type trainer;  /*!< Trainer presence                     */
    enum nsk_battery_type battery;  /*!< Battery/non-volatile memory presence */

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

#endif