; @file nsk_header_code.asm
; @brief Assembly source for generating the NES 2.0 header.
;
; Works together with `nsk_header_config.inc` to build
; a valid ROM header during compilation and linking.
;
; Part of the Nesokia project — MIT License.

.linecont +

.include "nsk_header_consts.inc"
.include "nsk_header_config.inc"

.include "subroutines/nsk_header_magic.inc"
.include "subroutines/memory/rom/nsk_header_prg_lsb.inc"
.include "subroutines/memory/rom/nsk_header_chr_lsb.inc"
.include "subroutines/nsk_header_flags6.inc"
.include "subroutines/nsk_header_flags7.inc"
.include "subroutines/nsk_header_mapper.inc"
;.include "subroutines/nsk_header_prgchrrom_msb.inc"
;.include "subroutines/nsk_header_prgram.inc"
;.include "subroutines/nsk_header_chrram.inc"
;.include "subroutines/nsk_header_timings.inc"
;.include "subroutines/nsk_header_vstype.inc"
;.include "subroutines/nsk_header_miscroms.inc"
;.include "subroutines/nsk_header_defexp.inc"

.segment NSK_SEGMENT_HEADER

; 0..3: iNES/NES2.0 "magic number"
nsk_header_magic

; 4: PRG ROM size LSB
nsk_header_prgrom_lsb                       \
    ::NSK_HEADER_PRGROM_SIZE

; 5: CHR ROM size LSB
nsk_header_chrrom_lsb                       \
    ::NSK_HEADER_CHRROM_SIZE

; 6: A set of flags to be put in the 6th byte
nsk_header_flags6                           \
    ::NSK_HEADER_NAMETABLE_HARDWIRED,       \
    ::NSK_HEADER_BATTERY,                   \
    ::NSK_HEADER_TRAINER,                   \
    ::NSK_HEADER_NAMETABLE_ALTERNATIVE,     \
    ::NSK_HEADER_MAPPER_ID

; 7: A set of flags to be put in the 7th byte
nsk_header_flags7                           \
    ::NSK_HEADER_CONSOLE_TYPE,              \
    ::NSK_HEADER_MAPPER_ID

; 8: The rest of the mapper ID and a submapper ID
nsk_header_mapper                           \
    ::NSK_HEADER_MAPPER_ID,                 \
    ::NSK_HEADER_SUBMAPPER_ID

; 9: PRG/CHR-ROM size MSB
;nsk_header_prgchrrom_msb                    \
;    ::NSK_HEADER_PRGROM_SIZE,               \
;    ::NSK_HEADER_CHRROM_SIZE

; 10: PRG-RAM/EEPROM size
;nsk_header_prgram                           \
;    ::NSK_HEADER_PRGRAM_SIZE,               \
;    ::NSK_HEADER_PRGNVRAM_SIZE

; 11: CHR-RAM size
;nsk_header_chrram                           \
;    ::NSK_HEADER_CHRRAM_SIZE,               \
;    ::NSK_HEADER_CHRNVRAM_SIZE

; 12: CPU/PPU timings
;nsk_header_timings                          \
;    ::NSK_HEADER_REGION,                    \
;    ::NSK_HEADER_CONSOLE_TYPE,              \
;    ::NSK_HEADER_MAPPER_ID

; 13: VS/extended system type
;nsk_header_vstype                           \
;    ::NSK_HEADER_VS_PPU,                    \
;    ::NSK_HEADER_VS_HARDWARE,               \
;    ::NSK_HEADER_CONSOLE_TYPE

; 14: Miscellaneous ROMs
;nsk_header_rom_misc                         \
;    ::NSK_HEADER_ROMS_MISC

; 15: Default expansion device
;nsk_header_device                           \
;    ::NSK_HEADER_EXPANSION_DEVICE

