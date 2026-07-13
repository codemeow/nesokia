# ca65/ld65 project build rules

ifneq ($(filter undefined default,$(origin CA)),)
CA := ca65
endif
ifneq ($(filter undefined default,$(origin LD)),)
LD := ld65
endif
CA_TOOL := $(firstword $(CA))
LD_TOOL := $(firstword $(LD))
ASM_MISSING_TOOLS := $(strip \
	$(if $(shell command -v $(CA_TOOL) 2>/dev/null),,$(CA_TOOL)) \
	$(if $(shell command -v $(LD_TOOL) 2>/dev/null),,$(LD_TOOL)) \
)
ASM_C_COMPILER := $(firstword $(foreach cc,gcc clang,$(shell command -v $(cc) 2>/dev/null)))

SOURCE_DIR ?= source
COMMON_DIR ?= ../common
MEMORY_CONFIG ?= memory/config.cfg

ASM_OUTPUT := $(DIR_BIN)/$(PROJECT_NAME)
ASM_OUTPUT_DIR := $(dir $(ASM_OUTPUT))
ASM_DBGFILE ?= $(DIR_BUILD)/$(notdir $(basename $(PROJECT_NAME))).dbg

ASM_CAFLAGS ?= -I $(COMMON_DIR) --bin-include-dir $(DIR_BUILD) -g
ASM_LDFLAGS ?= --dbgfile $(ASM_DBGFILE)

NSK_HEADER_DIR ?= $(DIR_ROOT)/utils/ines/header
NSK_COMMON_6502_DIR ?= $(DIR_ROOT)/utils/common/6502
NSK_HEADER_SOURCE := $(NSK_HEADER_DIR)/nsk_header_code.asm
NSK_HEADER_DEPS :=              \
	$(NSK_HEADER_SOURCE)        \
	$(NSK_COMMON_6502_DIR)/nsk_common_hw.inc \
	nsk_header_config.inc       \
	$(shell find "$(NSK_HEADER_DIR)/subroutines" -type f -name '*.inc' 2>/dev/null)
NSK_HEADER_OBJECT := $(DIR_BUILD)/header.o
NSK_HEADER_CAFLAGS ?= -I . -I $(NSK_HEADER_DIR) -I $(NSK_COMMON_6502_DIR)

ASM_SOURCES := $(shell find "$(SOURCE_DIR)" -type f -name '*.asm' 2>/dev/null | sort)
ASM_INCLUDES := $(shell find "$(SOURCE_DIR)" "$(COMMON_DIR)" -type f -name '*.inc' 2>/dev/null | sort)
ASM_OBJECTS := $(patsubst $(SOURCE_DIR)/%.asm,$(DIR_BUILD)/%.o,$(ASM_SOURCES))

CHR_BANKS ?=
CHR_OUTPUT_DIR ?= $(DIR_BUILD)/chr
PAL_OUTPUT_DIR ?= $(DIR_BUILD)/pal

NSK_UTIL_CONVERT ?= $(DIR_BIN)/nesokia-chr-convert
NSK_UTIL_CFG2EXP ?= $(DIR_ROOT)/scripts/conv/nsk_conv_file2exp.sh
ifeq ($(strip $(ASM_C_COMPILER)$(wildcard $(NSK_UTIL_CONVERT))),)
ifneq ($(strip $(CHR_BANKS)),)
ASM_MISSING_TOOLS += nesokia-chr-convert
endif
endif

chr_bank_required = $(if $(strip $($(1))),$($(1)),$(error Missing required CHR setting: $(1)))
chr_bank_png = $(call chr_bank_required,CHR_BANK_$(1)_PNG)
chr_bank_cfg = $(call chr_bank_required,CHR_BANK_$(1)_CFG)
chr_bank_left = $(call chr_bank_required,CHR_BANK_$(1)_LEFT)
chr_bank_right = $(call chr_bank_required,CHR_BANK_$(1)_RIGHT)
chr_bank_pal_back = $(strip $(CHR_BANK_$(1)_PAL_BACK))
chr_bank_pal_sprites = $(strip $(CHR_BANK_$(1)_PAL_SPRITES))
chr_bank_outputs = \
	$(call chr_bank_left,$(1))      \
	$(call chr_bank_right,$(1))     \
	$(call chr_bank_pal_back,$(1))  \
	$(call chr_bank_pal_sprites,$(1))

CHR_OUTPUT := $(strip $(foreach b,$(CHR_BANKS),$(call chr_bank_left,$(b)) $(call chr_bank_right,$(b))))
PAL_OUTPUT := $(strip $(foreach b,$(CHR_BANKS),$(call chr_bank_pal_back,$(b)) $(call chr_bank_pal_sprites,$(b))))
PAL_BACK := $(firstword $(strip $(foreach b,$(CHR_BANKS),$(call chr_bank_pal_back,$(b)))))
PAL_SPRITES := $(firstword $(strip $(foreach b,$(CHR_BANKS),$(call chr_bank_pal_sprites,$(b)))))
CHR_CATEGORY := $(DIR_BUILD)/.chr-category

CLEAN_ENTRIES += $(ASM_DBGFILE)

ifneq ($(ASM_MISSING_TOOLS),)
$(ASM_OUTPUT): | $(ASM_OUTPUT_DIR)
	@$(call print_category,Skipping)
	@$(call print_entry,Skipping $(PROJECT_NAME): missing ASM tool(s) $(ASM_MISSING_TOOLS))
else

.PHONY: chr
chr: $(CHR_OUTPUT) $(PAL_OUTPUT)

$(ASM_OUTPUT_DIR):
	@$(CMD_MKDIR) "$@"

$(DIR_BUILD):
	@$(CMD_MKDIR) "$@"

$(NSK_UTIL_CONVERT):
	@$(MAKE) -C "$(DIR_ROOT)/utils/chr/convert" build

$(ASM_OUTPUT): $(MEMORY_CONFIG) $(NSK_HEADER_OBJECT) $(ASM_OBJECTS) | $(ASM_OUTPUT_DIR)
	@$(call print_category,Linking)
	@$(LD) -C $(MEMORY_CONFIG) -o $@ $(NSK_HEADER_OBJECT) $(ASM_OBJECTS) $(ASM_LDFLAGS)
	@$(call print_entry,Linking $@)

$(NSK_HEADER_OBJECT): $(NSK_HEADER_DEPS) | $(DIR_BUILD)
	@$(call print_category,Compiling)
	@$(CMD_MKDIR) $(dir $@)
	@$(CA) $(ASM_CAFLAGS) $(NSK_HEADER_CAFLAGS) $(NSK_HEADER_SOURCE) -o $@
	@$(call print_entry,Compiling $(NSK_HEADER_SOURCE))

$(DIR_BUILD)/%.o: $(SOURCE_DIR)/%.asm $(ASM_INCLUDES) | $(DIR_BUILD)
	@$(CMD_MKDIR) $(dir $@)
	@$(CA) $(ASM_CAFLAGS) $< -o $@
	@$(call print_entry,Compiling $<)

ifneq ($(strip $(CHR_BANKS)),)
$(CHR_CATEGORY): | $(DIR_BUILD)
	@$(call print_category,Converting CHR)
	@$(CMD_TOUCH) $@

define CHR_BANK_RULE
$(call chr_bank_outputs,$(1)): $(call chr_bank_png,$(1)) $(call chr_bank_cfg,$(1)) $(NSK_UTIL_CONVERT) $(NSK_UTIL_CFG2EXP) | $(CHR_CATEGORY)
	@$$(CMD_MKDIR) $(sort $(dir $(call chr_bank_outputs,$(1))))
	@$$(NSK_UTIL_CONVERT) \
		-i $(call chr_bank_png,$(1)) \
		-L $(call chr_bank_left,$(1)) \
		-R $(call chr_bank_right,$(1)) \
		$(if $(call chr_bank_pal_back,$(1)),-B $(call chr_bank_pal_back,$(1))) \
		$(if $(call chr_bank_pal_sprites,$(1)),-S $(call chr_bank_pal_sprites,$(1))) \
		-e "$$$$($$(NSK_UTIL_CFG2EXP) $(call chr_bank_cfg,$(1)))" \
		> /dev/null
	@$$(call print_entry,Converting $(call chr_bank_png,$(1)))
endef

$(foreach b,$(CHR_BANKS),$(eval $(call CHR_BANK_RULE,$(b))))

$(NSK_HEADER_OBJECT) $(ASM_OBJECTS): | $(CHR_OUTPUT) $(PAL_OUTPUT)

$(DIR_BUILD)/chr/nsk_chr_banks.o: $(CHR_OUTPUT)
ifneq ($(strip $(PAL_BACK)),)
$(DIR_BUILD)/objects/tiles/palettes/nsk_tiles_palette.o: $(PAL_BACK)
endif
ifneq ($(strip $(PAL_SPRITES)),)
$(DIR_BUILD)/objects/sprites/palettes/nsk_sprites_palette.o: $(PAL_SPRITES)
endif
endif

endif
