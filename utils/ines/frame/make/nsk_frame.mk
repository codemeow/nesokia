# @file nsk_frame.mk
# @brief Build fragment for Nesokia iNES frame sources
#
# Part of the Nesokia project — MIT License.

ifndef __NSK_FRAME_MK__
__NSK_FRAME_MK__ := 1

NSK_FRAME_SAVED_DEFAULT_GOAL := $(.DEFAULT_GOAL)

# Required project settings
ifeq ($(strip $(NSK_FRAME_CONFIG_DIR)),)
$(error NSK_FRAME_CONFIG_DIR must name the project frame configuration directory)
endif

ifeq ($(strip $(NSK_FRAME_BUILD_DIR)),)
$(error NSK_FRAME_BUILD_DIR must name the frame object output directory)
endif

# Optional project settings
NSK_FRAME_CA ?= ca65
NSK_FRAME_CAFLAGS ?=

# The public fragment is staged in `bin/make` and locates framework sources
# relative to the Nesokia root. Projects never supply paths below that root.
NSK_FRAME_MK_FILE := $(abspath $(lastword $(MAKEFILE_LIST)))
NSK_FRAME_ROOT := $(abspath $(dir $(NSK_FRAME_MK_FILE))/../..)
NSK_FRAME_SOURCE_DIR := $(NSK_FRAME_ROOT)/utils/ines/frame/source
NSK_FRAME_COMMON_6502_DIR := $(NSK_FRAME_ROOT)/bin/includes/6502

ifeq ($(wildcard $(NSK_FRAME_SOURCE_DIR)),)
$(error NSK frame source directory is missing: $(NSK_FRAME_SOURCE_DIR))
endif

ifeq ($(wildcard $(NSK_FRAME_COMMON_6502_DIR)),)
$(error Nesokia SDK includes are missing; build nesokia-ines-frame first)
endif

NSK_FRAME_CONFIG_DIR := $(abspath $(NSK_FRAME_CONFIG_DIR))
NSK_FRAME_BUILD_DIR := $(abspath $(NSK_FRAME_BUILD_DIR))

# Configurations normally live in the root directory, with the header
# configuration in its `header` subdirectory. Projects may replace this list
# when using another layout.
NSK_FRAME_CONFIG_INCLUDE_DIRS ?= \
	$(NSK_FRAME_CONFIG_DIR) \
	$(NSK_FRAME_CONFIG_DIR)/header

# Every runtime source is compiled. Frame self-tests live outside `source` and
# are intentionally excluded from project builds.
NSK_FRAME_SOURCES := $(shell find "$(NSK_FRAME_SOURCE_DIR)" -type f -name '*.asm' | sort)
NSK_FRAME_INCLUDES := $(shell find "$(NSK_FRAME_SOURCE_DIR)" -type f -name '*.inc' | sort)
NSK_FRAME_CONFIG_INCLUDES := $(shell find "$(NSK_FRAME_CONFIG_DIR)" -type f -name '*.inc' | sort)
NSK_FRAME_COMMON_INCLUDES := $(shell find "$(NSK_FRAME_COMMON_6502_DIR)" -type f -name '*.inc' | sort)
NSK_FRAME_DEPENDENCIES := \
	$(NSK_FRAME_INCLUDES) \
	$(NSK_FRAME_CONFIG_INCLUDES) \
	$(NSK_FRAME_COMMON_INCLUDES)

NSK_FRAME_OBJECTS := $(patsubst $(NSK_FRAME_SOURCE_DIR)/%.asm,$(NSK_FRAME_BUILD_DIR)/%.o,$(NSK_FRAME_SOURCES))
NSK_FRAME_INCLUDE_DIRS := \
	$(NSK_FRAME_CONFIG_INCLUDE_DIRS) \
	$(NSK_FRAME_COMMON_6502_DIR)
NSK_FRAME_INCLUDE_FLAGS := $(foreach dir,$(NSK_FRAME_INCLUDE_DIRS),-I $(dir))

.PHONY: frame
frame: $(NSK_FRAME_OBJECTS)

$(NSK_FRAME_BUILD_DIR)/%.o: $(NSK_FRAME_SOURCE_DIR)/%.asm $(NSK_FRAME_DEPENDENCIES)
	@mkdir -p "$(dir $@)"
	$(NSK_FRAME_CA) $(NSK_FRAME_CAFLAGS) $(NSK_FRAME_INCLUDE_FLAGS) -o "$@" "$<"

ifeq ($(strip $(NSK_FRAME_SAVED_DEFAULT_GOAL)),)
.DEFAULT_GOAL :=
else
.DEFAULT_GOAL := $(NSK_FRAME_SAVED_DEFAULT_GOAL)
endif

endif
