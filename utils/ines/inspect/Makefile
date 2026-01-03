# Output binary name
TARGET  := nesokia-ines-inspect

# Version data
VER_MAJOR := 0
VER_MINOR := 3
VER_BUILD := 9

# Common root directory
DIR_ROOT := ../../..

# Build directory for object files
DIR_BUILD := $(DIR_ROOT)/build/$(TARGET)
# Output binary directory
DIR_BIN := $(DIR_ROOT)/bin
# Extra headers to cover platform-specific stuff
DIR_INCLUDES := $(DIR_ROOT)/utils/common/includes
# Extra common sources
DIR_COMMON := $(DIR_ROOT)/utils/common/utils
# Extra headers access
DIR_COMMON_X := $(DIR_ROOT)/utils/common/xmacro


# Version defines
VER_STRING := \
	-DNSK_VERSION_MAJOR=$(VER_MAJOR)\
	-DNSK_VERSION_MINOR=$(VER_MINOR)\
	-DNSK_VERSION_BUILD=$(VER_BUILD)
# Program name string define
NAME_STRING := -DNSK_PROGRAM_NAME=\"$(TARGET)\"

# List of additional headers
LIST_HEADERS := $(shell find $(DIR_INCLUDES) -type f -name '*.h')
# Compile options for additional headers
INCLUDE_HEADERS := $(addprefix -include ,$(LIST_HEADERS))

# Compiler and flags
CC      := gcc
#CC := x86_64-w64-mingw32-gcc -mno-ms-bitfields -static-libgcc

# Compilation flags
CFLAGS  := -std=c11 -Wall -Wextra -Werror -O0 -g \
	$(VER_STRING) $(NAME_STRING) \
	$(INCLUDE_HEADERS) -I$(DIR_COMMON_X) -I$(DIR_COMMON)
# Linking flags
LDFLAGS := -lm -lpng

# List of source files
SRCS_COMMON  := $(shell find $(DIR_COMMON) -type f -name '*.c')
SRCS_PROJECT := $(shell find . -type f -name '*.c')

# Automatically derive object file paths inside build/
OBJS_COMMON  := $(patsubst $(DIR_COMMON)/%.c,$(DIR_BUILD)/common/%.c.o,$(SRCS_COMMON))
OBJS_PROJECT := $(SRCS_PROJECT:%=$(DIR_BUILD)/%.o)


# Default target
all: $(DIR_BIN)/$(TARGET)

# Link final binary
$(DIR_BIN)/$(TARGET): $(OBJS_COMMON) $(OBJS_PROJECT) | $(DIR_BIN)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Create build directory
$(DIR_BUILD):
	mkdir -p $@

# Create bin directory
$(DIR_BIN):
	mkdir -p $@

# Compile rule — auto-create build subdirectories
$(DIR_BUILD)/%.c.o: %.c | $(DIR_BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile common files
$(DIR_BUILD)/common/%.c.o: $(DIR_COMMON)/%.c | $(DIR_BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(DIR_BUILD)
	rm -rf $(DIR_BIN)

# Target list
.PHONY: all clean

# Default target
.DEFAULT_GOAL := all
