; @file nsk_mapper_mmc3.asm
; @brief MMC3 mapper initialization source
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_FRAME_MAPPER_ASM__
::__NSK_FRAME_MAPPER_ASM__ = 1

.linecont +

.include "nsk_common_hw.inc"
.include "nsk_common_mapper.inc"
.include "mapper/nsk_frame_mapper.inc"
.include "../nsk_frame_defaults.inc"

.if ::NSK_FEATURE_MAPPER = 1
.if .defined(NSK_HEADER_MAPPER_ID)
.if ::NSK_HEADER_MAPPER_ID = 4
.if .defined(NSK_HEADER_SUBMAPPER_ID)
.if ::NSK_HEADER_SUBMAPPER_ID = 0

.assert (NSK_MMC3_INIT_MODE & NSKMP::MMC3::SELECT::MODE_MASK) = NSK_MMC3_INIT_MODE, \
    error, "NSK_MMC3_INIT_MODE contains unsupported bits"

.assert NSK_MMC3_INIT_R0_2K >= 0 .and NSK_MMC3_INIT_R0_2K <= NSKMP::MMC3::CHR::BANK_2K_INDEX_MAX, \
    error, "NSK_MMC3_INIT_R0_2K must be a 2 KiB CHR bank index in range 0-127"
.assert NSK_MMC3_INIT_R1_2K >= 0 .and NSK_MMC3_INIT_R1_2K <= NSKMP::MMC3::CHR::BANK_2K_INDEX_MAX, \
    error, "NSK_MMC3_INIT_R1_2K must be a 2 KiB CHR bank index in range 0-127"

.assert NSK_MMC3_INIT_R2_1K >= 0 .and NSK_MMC3_INIT_R2_1K <= NSKMP::MMC3::CHR::BANK_1K_INDEX_MAX, \
    error, "NSK_MMC3_INIT_R2_1K must be a 1 KiB CHR bank index in range 0-255"
.assert NSK_MMC3_INIT_R3_1K >= 0 .and NSK_MMC3_INIT_R3_1K <= NSKMP::MMC3::CHR::BANK_1K_INDEX_MAX, \
    error, "NSK_MMC3_INIT_R3_1K must be a 1 KiB CHR bank index in range 0-255"
.assert NSK_MMC3_INIT_R4_1K >= 0 .and NSK_MMC3_INIT_R4_1K <= NSKMP::MMC3::CHR::BANK_1K_INDEX_MAX, \
    error, "NSK_MMC3_INIT_R4_1K must be a 1 KiB CHR bank index in range 0-255"
.assert NSK_MMC3_INIT_R5_1K >= 0 .and NSK_MMC3_INIT_R5_1K <= NSKMP::MMC3::CHR::BANK_1K_INDEX_MAX, \
    error, "NSK_MMC3_INIT_R5_1K must be a 1 KiB CHR bank index in range 0-255"

.assert NSK_MMC3_INIT_R6_8K >= 0 .and NSK_MMC3_INIT_R6_8K <= NSKMP::MMC3::PRG::BANK_INDEX_MAX, \
    error, "NSK_MMC3_INIT_R6_8K must be an 8 KiB PRG bank index in range 0-63"
.assert NSK_MMC3_INIT_R7_8K >= 0 .and NSK_MMC3_INIT_R7_8K <= NSKMP::MMC3::PRG::BANK_INDEX_MAX, \
    error, "NSK_MMC3_INIT_R7_8K must be an 8 KiB PRG bank index in range 0-63"

.assert (NSK_MMC3_INIT_MIRRORING & NSKMP::MMC3::MIRRORING::MODE_MASK) = NSK_MMC3_INIT_MIRRORING, \
    error, "NSK_MMC3_INIT_MIRRORING contains unsupported bits"

.if .defined(NSK_MMC3_INIT_PRG_RAM)
    .assert (NSK_MMC3_INIT_PRG_RAM & NSKMP::MMC3::PRG_RAM::CONTROL_MASK) = NSK_MMC3_INIT_PRG_RAM, \
        error, "NSK_MMC3_INIT_PRG_RAM contains unsupported bits"
.endif

.segment NSK_SEGMENT_MAPPER

; @brief Selects an MMC3 bank register and writes its initial value.
.macro nsk_mmc3_init_bank bank_register, bank_value
    lda #(NSK_MMC3_INIT_MODE | bank_register)
    sta NSKMP::MMC3::REGISTER::BANK_SELECT
    lda #bank_value
    sta NSKMP::MMC3::REGISTER::BANK_DATA
.endmacro

; @brief Initializes the MMC3 mapping state for a reset.
; @details Leaves scanline IRQs disabled. Their setup belongs to the IRQ subsystem.
.export nsk_mapper_init
.proc nsk_mapper_init
    lda #0
    sta NSKMP::MMC3::REGISTER::IRQ_DISABLE

    nsk_mmc3_init_bank NSKMP::MMC3::BANK::R0, (NSK_MMC3_INIT_R0_2K << 1)
    nsk_mmc3_init_bank NSKMP::MMC3::BANK::R1, (NSK_MMC3_INIT_R1_2K << 1)
    nsk_mmc3_init_bank NSKMP::MMC3::BANK::R2, NSK_MMC3_INIT_R2_1K
    nsk_mmc3_init_bank NSKMP::MMC3::BANK::R3, NSK_MMC3_INIT_R3_1K
    nsk_mmc3_init_bank NSKMP::MMC3::BANK::R4, NSK_MMC3_INIT_R4_1K
    nsk_mmc3_init_bank NSKMP::MMC3::BANK::R5, NSK_MMC3_INIT_R5_1K
    nsk_mmc3_init_bank NSKMP::MMC3::BANK::R6, NSK_MMC3_INIT_R6_8K
    nsk_mmc3_init_bank NSKMP::MMC3::BANK::R7, NSK_MMC3_INIT_R7_8K

    lda #NSK_MMC3_INIT_MIRRORING
    sta NSKMP::MMC3::REGISTER::MIRRORING

.if .defined(NSK_MMC3_INIT_PRG_RAM)
    lda #NSK_MMC3_INIT_PRG_RAM
    sta NSKMP::MMC3::REGISTER::PRG_RAM_PROTECT
.endif

    rts
.endproc

.endif
.endif
.endif
.endif
.endif

.endif
