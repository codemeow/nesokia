override PROJECT_NAME := nesokia-ines-frame

# Common root directory
DIR_ROOT := ../../..

include $(DIR_ROOT)/scripts/make/common.mk
include $(DIR_ROOT)/scripts/make/lang/dummy.mk

SDK_MAKE_DIR := $(DIR_ROOT)/bin/make
SDK_COMMON_SOURCE_DIR := $(DIR_ROOT)/utils/common/6502
SDK_COMMON_TARGET_DIR := $(DIR_ROOT)/bin/includes/6502
SDK_COMMON_SOURCES := $(sort $(wildcard $(SDK_COMMON_SOURCE_DIR)/*.inc))
SDK_FRAME_MAKE_SOURCE := make/nsk_frame.mk
SDK_FRAME_MAKE_TARGET := $(SDK_MAKE_DIR)/nsk_frame.mk

.PHONY: nsk-sdk-common-sync
nsk-sdk-common-sync:
	@$(CMD_MKDIR) "$(SDK_COMMON_TARGET_DIR)"
	@set -e; \
	for source in $(SDK_COMMON_SOURCES); do \
		target="$(SDK_COMMON_TARGET_DIR)/$$(basename "$$source")"; \
		if ! cmp -s "$$source" "$$target"; then \
			$(CMD_CP) "$$source" "$$target"; \
		fi; \
	done; \
	for target in "$(SDK_COMMON_TARGET_DIR)"/*.inc; do \
		[ -e "$$target" ] || continue; \
		source="$(SDK_COMMON_SOURCE_DIR)/$$(basename "$$target")"; \
		[ -f "$$source" ] || $(CMD_RM) "$$target"; \
	done

$(SDK_FRAME_MAKE_TARGET): $(SDK_FRAME_MAKE_SOURCE)
	@$(CMD_MKDIR) "$(SDK_MAKE_DIR)"
	@if ! cmp -s "$<" "$@"; then $(CMD_CP) "$<" "$@"; fi

all build: nsk-sdk-common-sync $(SDK_FRAME_MAKE_TARGET)

CLEAN_ENTRIES += $(SDK_FRAME_MAKE_TARGET)
