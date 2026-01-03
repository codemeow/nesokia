
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
NAME_STRING := -DNSK_PROGRAM_NAME=\"$(PROJECT_NAME)\"

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

# List of source files
SRCS_COMMON  := $(shell find $(DIR_COMMON) -type f -name '*.c')
SRCS_PROJECT := $(shell find . -type f -name '*.c')

# Automatically derive object file paths inside build/
OBJS_COMMON  := $(patsubst $(DIR_COMMON)/%.c,$(DIR_BUILD)/common/%.c.o,$(SRCS_COMMON))
OBJS_PROJECT := $(SRCS_PROJECT:%=$(DIR_BUILD)/%.o)

# Linking
$(DIR_BIN):
	@mkdir -p $@

$(DIR_BIN)/$(PROJECT_NAME): $(OBJS_COMMON) $(OBJS_PROJECT) | $(DIR_BIN)
	@$(call print_category,Linking)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@$(call print_entry,Linking $@)

# Compile
#.PHONY: compile-pre
#compile-pre:
#	@$(call print_category,Compiling)

COMPILE_STAMP := $(DIR_BUILD)/.compile-stamp

$(COMPILE_STAMP): | $(DIR_BUILD)
	@$(call print_category,Compiling)
	@touch $@

$(DIR_BUILD):
	@mkdir -p $@

$(DIR_BUILD)/%.c.o: %.c | $(DIR_BUILD)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(call print_entry,Compiling $<)

$(DIR_BUILD)/common/%.c.o: $(DIR_COMMON)/%.c $(COMPILE_STAMP) | $(DIR_BUILD)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(call print_entry,Compiling $<)
