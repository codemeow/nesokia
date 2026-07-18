; @file nsk_common_mapper.inc
; @brief Include module for the NES mappers constants
;
; Part of the Nesokia project — MIT License.

.ifndef ::__NSK_COMMON_MAPPER__
::__NSK_COMMON_MAPPER__ = 1

.linecont +

; @brief Mapper-related constants scope
.scope NSKMP

    ; @brief MMC3-related constants
    .scope MMC3

        ; @brief Memory-mapped MMC3 registers
        .scope REGISTER
            BANK_SELECT     = $8000 ; Select R0-R7 and PRG/CHR modes
            BANK_DATA       = $8001 ; Write data to selected R0-R7 register

            MIRRORING       = $A000 ; Nametable mirroring mode
            PRG_RAM_PROTECT = $A001 ; PRG RAM enable and write protection

            IRQ_LATCH       = $C000 ; IRQ counter reload value
            IRQ_RELOAD      = $C001 ; Request IRQ counter reload

            IRQ_DISABLE     = $E000 ; Disable and acknowledge IRQ
            IRQ_ENABLE      = $E001 ; Enable IRQ generation
        .endscope

        ; @brief Bank-select register bit masks
        ; @details $8000 format: CPxx xRRR
        ;          C   - CHR mode
        ;          P   - PRG mode
        ;          RRR - R0-R7 register number
        .scope SELECT
            REGISTER_MASK = %00000111 ; R0-R7 selection mask
            PRG_MODE_MASK = %01000000 ; PRG banking mode bit
            CHR_MODE_MASK = %10000000 ; CHR banking mode bit
            MODE_MASK     = %11000000 ; PRG and CHR mode bits
        .endscope

        ; @brief Internal MMC3 bank registers
        .scope BANK
            R0 = 0 ; 2 KiB CHR bank
            R1 = 1 ; 2 KiB CHR bank

            R2 = 2 ; 1 KiB CHR bank
            R3 = 3 ; 1 KiB CHR bank
            R4 = 4 ; 1 KiB CHR bank
            R5 = 5 ; 1 KiB CHR bank

            R6 = 6 ; 8 KiB PRG bank
            R7 = 7 ; 8 KiB PRG bank
        .endscope

        ; @brief CHR banking constants
        .scope CHR
            BANK_1K_SIZE = $0400 ; R2-R5 bank size
            BANK_2K_SIZE = $0800 ; R0-R1 bank size
            TABLE_SIZE   = $1000 ; Pattern table size

            ; @brief Largest selectable CHR bank index for R2-R5
            BANK_1K_INDEX_MAX = $ff

            ; @brief Largest 2 KiB CHR bank index accepted for R0-R1
            ; @details The hardware receives this value shifted left by one.
            BANK_2K_INDEX_MAX = BANK_1K_INDEX_MAX >> 1

            ; @brief Left table uses 2 KiB banks, right table uses 1 KiB banks
            MODE_L2R1 = %00000000

            ; @brief Left table uses 1 KiB banks, right table uses 2 KiB banks
            MODE_L1R2 = %10000000
        .endscope

        ; @brief PRG banking constants
        .scope PRG
            BANK_SIZE = $2000 ; PRG bank size: 8 KiB

            RAM_START = $6000 ; PRG RAM start address
            RAM_END   = $7FFF ; PRG RAM end address

            SLOT_0 = $8000 ; CPU $8000-$9FFF
            SLOT_1 = $A000 ; CPU $A000-$BFFF
            SLOT_2 = $C000 ; CPU $C000-$DFFF
            SLOT_3 = $E000 ; CPU $E000-$FFFF

            ; @brief PRG layout: R6, R7, penultimate, last
            MODE_67PL = %00000000

            ; @brief PRG layout: penultimate, R7, R6, last
            MODE_P76L = %01000000

            ; @brief PRG ROM bank-number indexing
            BANK_INDEX_MAX = 63
        .endscope

        ; @brief Nametable mirroring constants
        .scope MIRRORING
            HORIZONTAL = %00000001 ; Horizontal nametable mirroring
            VERTICAL   = %00000000 ; Vertical nametable mirroring

            MODE_MASK  = %00000001 ; Mirroring mode bit mask
        .endscope

        ; @brief PRG RAM control constants
        ; @note Basic MMC3-only
        ; @details $A001 bit 7 enables PRG RAM.
        ;          $A001 bit 6 protects PRG RAM from writes.
        .scope PRG_RAM
            DISABLED            = %00000000 ; PRG RAM disabled
            READ_WRITE          = %10000000 ; PRG RAM enabled and writable
            READ_ONLY           = %11000000 ; PRG RAM enabled and write-protected

            WRITE_ENABLE        = %00000000 ; Allow PRG RAM writes
            WRITE_PROTECT       = %01000000 ; Prevent PRG RAM writes
            ENABLE              = %10000000 ; Enable PRG RAM

            WRITE_PROTECT_MASK  = %01000000 ; Write-protection bit mask
            ENABLE_MASK         = %10000000 ; PRG RAM enable bit mask
            CONTROL_MASK        = %11000000 ; Complete control bit mask
        .endscope

    .endscope

.endscope

.endif
