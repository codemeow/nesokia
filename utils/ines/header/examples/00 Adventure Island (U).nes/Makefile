# Output binary name
TARGET := example.nes

# Compiler
CA := ca65
# Linker
LD := ld65
# Remove utility
RM := rm -f
# Directory create utility
MKDIR := mkdir -p

# Game source directory
SOURCE_DIR := source
# Build directory
BUILD_DIR  := build
# Common examples files dir
COMMON_DIR := ../common
# Nesokia root dir
ROOT_DIR   := ../../../../..
# Nesokia utilities bin
ROOT_BIN_DIR := $(ROOT_DIR)/bin
# Nesokia scripts bin
ROOT_SCRIPTS_DIR := $(ROOT_DIR)/scripts

# Nesokiad utilities
# CHR converter
NSK_UTIL_CONVERT := $(ROOT_BIN_DIR)/nesokia-chr-convert
# CFG to explicit palette list converter
NSK_UTIL_CFG2EXP := $(ROOT_SCRIPTS_DIR)/conv/nsk_conv_file2exp.sh

# Nesokia source
NSK_SOURCE := ../../nsk_header_code.asm
# Nesokia header dependencies
NSK_DEPS   := $(NSK_SOURCE) \
	../../nsk_header_consts.inc \
	nsk_header_config.inc \
	$(shell find ../../subroutines -type f -name '*.inc')
# Nesokia target object
NSK_OBJECT := $(BUILD_DIR)/header.o
# Nesokia compilation options
NSK_FLAGS  := -I .


# Test game source
GAME_SOURCE := $(shell find $(SOURCE_DIR) -type f -name '*.asm')
# Test game object
GAME_OBJECT := $(patsubst $(SOURCE_DIR)/%.asm,$(BUILD_DIR)/%.o,$(GAME_SOURCE))
# Test game linker script
GAME_MEMORY := memory/config.cfg
# All source subdirs
GAME_DIRS   := $(sort $(dir $(GAME_OBJECT)))

# CHR ROM source banks
CHR_BANKS := 0 1 2 3
# CHR ROM source files
CHR_PNG := $(addprefix chrrom/png/bank-,$(addsuffix .png,$(CHR_BANKS)))
CHR_CFG := $(addprefix chrrom/png/bank-,$(addsuffix .cfg,$(CHR_BANKS)))
# CHR ROM generated files
CHR_LEFT  := $(addprefix $(BUILD_DIR)/chr/bank-,$(addsuffix .0000.chr,$(CHR_BANKS)))
CHR_RIGHT := $(addprefix $(BUILD_DIR)/chr/bank-,$(addsuffix .1000.chr,$(CHR_BANKS)))
CHR_OUTPUT := $(CHR_LEFT) $(CHR_RIGHT)
# Generated palettes
PAL_OUTPUT := $(BUILD_DIR)/pal/back.pal $(BUILD_DIR)/pal/sprites.pal

# Additional compiler flags
CFLAGS := -I $(COMMON_DIR) -g
# Additional linker flags
LDFLAGS := --dbgfile $(TARGET:.nes=.dbg)

# Target: all
.PHONY: all
all: $(TARGET)

# Target: convert CHRs
.PHONY: chr
chr: $(CHR_OUTPUT) $(PAL_OUTPUT)

$(BUILD_DIR)/chr/bank-0.0000.chr $(BUILD_DIR)/chr/bank-0.1000.chr $(BUILD_DIR)/pal/back.pal $(BUILD_DIR)/pal/sprites.pal: chrrom/png/bank-0.png chrrom/png/bank-0.cfg $(NSK_UTIL_CONVERT) $(NSK_UTIL_CFG2EXP)
	$(MKDIR) $(BUILD_DIR)/pal
	$(MKDIR) $(BUILD_DIR)/chr
	$(NSK_UTIL_CONVERT) \
		-i chrrom/png/bank-0.png \
		-L $(BUILD_DIR)/chr/bank-0.0000.chr \
		-R $(BUILD_DIR)/chr/bank-0.1000.chr \
		-B $(BUILD_DIR)/pal/back.pal \
		-S $(BUILD_DIR)/pal/sprites.pal \
		-e "$$($(NSK_UTIL_CFG2EXP) chrrom/png/bank-0.cfg)"

$(BUILD_DIR)/chr/bank-1.0000.chr $(BUILD_DIR)/chr/bank-1.1000.chr: chrrom/png/bank-1.png chrrom/png/bank-1.cfg $(NSK_UTIL_CONVERT) $(NSK_UTIL_CFG2EXP)
	$(MKDIR) $(BUILD_DIR)/chr
	$(NSK_UTIL_CONVERT) \
		-i chrrom/png/bank-1.png \
		-L $(BUILD_DIR)/chr/bank-1.0000.chr \
		-R $(BUILD_DIR)/chr/bank-1.1000.chr \
		-e "$$($(NSK_UTIL_CFG2EXP) chrrom/png/bank-1.cfg)"

$(BUILD_DIR)/chr/bank-2.0000.chr $(BUILD_DIR)/chr/bank-2.1000.chr: chrrom/png/bank-2.png chrrom/png/bank-2.cfg $(NSK_UTIL_CONVERT) $(NSK_UTIL_CFG2EXP)
	$(MKDIR) $(BUILD_DIR)/chr
	$(NSK_UTIL_CONVERT) \
		-i chrrom/png/bank-2.png \
		-L $(BUILD_DIR)/chr/bank-2.0000.chr \
		-R $(BUILD_DIR)/chr/bank-2.1000.chr \
		-e "$$($(NSK_UTIL_CFG2EXP) chrrom/png/bank-2.cfg)"

$(BUILD_DIR)/chr/bank-3.0000.chr $(BUILD_DIR)/chr/bank-3.1000.chr: chrrom/png/bank-3.png chrrom/png/bank-3.cfg $(NSK_UTIL_CONVERT) $(NSK_UTIL_CFG2EXP)
	$(MKDIR) $(BUILD_DIR)/chr
	$(NSK_UTIL_CONVERT) \
		-i chrrom/png/bank-3.png \
		-L $(BUILD_DIR)/chr/bank-3.0000.chr \
		-R $(BUILD_DIR)/chr/bank-3.1000.chr \
		-e "$$($(NSK_UTIL_CFG2EXP) chrrom/png/bank-3.cfg)"


# Target: ROM
$(TARGET): $(GAME_MEMORY) $(NSK_OBJECT) $(GAME_OBJECT)
	$(LD) -C $(GAME_MEMORY) -o $@ $(NSK_OBJECT) $(GAME_OBJECT) $(LDFLAGS)

# Target: Nesokia header object
$(NSK_OBJECT): $(NSK_DEPS)
	@$(MKDIR) $(dir $@)
	$(CA) $(CFLAGS) $(NSK_FLAGS) $(NSK_SOURCE) -o $@

# CHR banks object includes generated CHR binaries.
$(BUILD_DIR)/chr/nsk_chr_banks.o: $(CHR_OUTPUT)

# Subdirectory for the object target
$(GAME_DIRS):
	$(MKDIR) $@

# Target: Example game code
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.asm
	@$(MKDIR) $(dir $@)
	$(CA) $(CFLAGS) $< -o $@

# Target: clean
.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(TARGET) $(TARGET:.nes=.dbg)

# Default target
.DEFAULT_GOAL := all
