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

# Nesokiad utilities
NSK_UTIL_CONVERT := $(ROOT_BIN_DIR)/nesokia-chr-convert

# Nesokia source
NSK_SOURCE := ../../nsk_header_code.asm
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
GAME_DIRS   := $(sort $(dir $(GAME_OBJECTS)))

# Additional compiler flags
CFLAGS := -I $(COMMON_DIR)
# Additional linker flags
LDFLAGS :=

# Target: all
.PHONY: all
all: $(TARGET)

# Target: convert CHRs
# @todo: Make this one non-phony target with deps of.png and add .chrsas deps for asm
# to fix the "changed png -> including asm wasn't rebuilt"
.PHONY: chr
chr:
	$(MKDIR) $(BUILD_DIR)/pal
	$(MKDIR) $(BUILD_DIR)/chr
	$(NSK_UTIL_CONVERT) \
		-i chrrom/png/bank-0.png \
		-L $(BUILD_DIR)/chr/bank-0.0000.chr \
		-R $(BUILD_DIR)/chr/bank-0.1000.chr \
		-B $(BUILD_DIR)/pal/back.pal \
		-S $(BUILD_DIR)/pal/sprites.pal
	$(NSK_UTIL_CONVERT) \
		-i chrrom/png/bank-1.png \
		-L $(BUILD_DIR)/chr/bank-1.0000.chr \
		-R $(BUILD_DIR)/chr/bank-1.1000.chr
	$(NSK_UTIL_CONVERT) \
		-i chrrom/png/bank-2.png \
		-L $(BUILD_DIR)/chr/bank-2.0000.chr \
		-R $(BUILD_DIR)/chr/bank-2.1000.chr
	$(NSK_UTIL_CONVERT) \
		-i chrrom/png/bank-3.png \
		-L $(BUILD_DIR)/chr/bank-3.0000.chr \
		-R $(BUILD_DIR)/chr/bank-3.1000.chr


# Target: ROM
$(TARGET): chr $(NSK_OBJECT) $(GAME_OBJECT)
	$(LD) -C $(GAME_MEMORY) -o $@ $(NSK_OBJECT) $(GAME_OBJECT) $(LDFLAGS)

# Target: Nesokia header object
$(NSK_OBJECT): $(NSK_SOURCE)
	$(CA) $(CFLAGS) $(NSK_FLAGS) $(NSK_SOURCE) -o $@

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
	$(RM) $(NSK_OBJECT)
	$(RM) $(GAME_OBJECT)
	$(RM) $(TARGET)

# Default target
.DEFAULT_GOAL := all