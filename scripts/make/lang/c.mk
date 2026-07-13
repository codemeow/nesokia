
# Extra headers to cover platform-specific stuff
DIR_INCLUDES := $(DIR_ROOT)/utils/common/c/includes
# Extra common sources
DIR_COMMON := $(DIR_ROOT)/utils/common/c/utils
# Extra headers access
DIR_COMMON_X := $(DIR_ROOT)/utils/common/c/xmacro

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
ifeq ($(origin CC),default)
CC := $(firstword $(foreach cc,gcc clang,$(shell command -v $(cc) 2>/dev/null)))
endif
#CC := x86_64-w64-mingw32-gcc -mno-ms-bitfields -static-libgcc
C_COMPILER := $(firstword $(CC))
C_MISSING_TOOL := $(if $(strip $(C_COMPILER)),$(if $(shell command -v $(C_COMPILER) 2>/dev/null),,$(C_COMPILER)),gcc/clang)

ifneq ($(strip $(C_MISSING_TOOL)),)
$(DIR_BIN)/$(PROJECT_NAME):
	@$(call print_category,Skipping)
	@$(call print_entry,Skipping $(PROJECT_NAME): missing C compiler $(C_MISSING_TOOL))
else

# Compilation flags
CFLAGS  += -std=c11 -Wall -Wextra -Werror -O0 -g \
	$(VER_STRING) $(NAME_STRING) \
	$(INCLUDE_HEADERS) -I$(DIR_COMMON_X) -I$(DIR_COMMON)

include $(DIR_ROOT)/scripts/make/lang/c/modules.mk

# List of source files
SRCS_COMMON  := $(foreach m,$(COMMON_MODULES_RESOLVED), \
	$(shell find $(DIR_COMMON)/$(m) -type f -name '*.c') \
)
SRCS_PROJECT := $(shell find . -type f -name '*.c')

# Automatically derive object file paths inside build/
OBJS_COMMON  := $(patsubst $(DIR_COMMON)/%.c,$(DIR_BUILD)/common/%.c.o,$(SRCS_COMMON))
OBJS_PROJECT := $(SRCS_PROJECT:%=$(DIR_BUILD)/%.o)

# Linking
$(DIR_BIN):
	@$(CMD_MKDIR) -p $@

$(DIR_BIN)/$(PROJECT_NAME): $(OBJS_COMMON) $(OBJS_PROJECT) | $(DIR_BIN)
	@$(call print_category,Linking)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(COMMON_LDLIBS)
	@$(call print_entry,Linking $@)

# Bug: doesn't show "Compiling" in case of incremental
COMPILE_STAMP := $(DIR_BUILD)/.compile-stamp

$(COMPILE_STAMP): | $(DIR_BUILD)
	@$(call print_category,Compiling)
	@$(CMD_TOUCH) $@

$(DIR_BUILD):
	@$(CMD_MKDIR) -p $@

$(DIR_BUILD)/%.c.o: %.c | $(DIR_BUILD)
	@$(CMD_MKDIR) -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(call print_entry,Compiling $<)

$(DIR_BUILD)/common/%.c.o: $(DIR_COMMON)/%.c $(COMPILE_STAMP) | $(DIR_BUILD)
	@$(CMD_MKDIR) -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(call print_entry,Compiling $<)

endif
