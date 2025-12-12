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
all: $(TARGET)

# Target: ROM
$(TARGET): $(NSK_OBJECT) $(GAME_OBJECT)
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
clean:
	$(RM) $(NSK_OBJECT)
	$(RM) $(GAME_OBJECT)
	$(RM) $(TARGET)

# Target list
.PHONY: all clean

# Default target
.DEFAULT_GOAL := all