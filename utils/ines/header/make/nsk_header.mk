# @file nsk_header.mk
# @brief Build fragment for the Nesokia iNES header source
#
# Part of the Nesokia project — MIT License.

ifndef __NSK_HEADER_MK__
__NSK_HEADER_MK__ := 1

NSK_HEADER_SAVED_DEFAULT_GOAL := $(.DEFAULT_GOAL)

# Required project settings
ifeq ($(strip $(NSK_HEADER_CONFIG_DIR)),)
$(error NSK_HEADER_CONFIG_DIR must name the project header configuration directory)
endif

ifeq ($(strip $(NSK_HEADER_BUILD_DIR)),)
$(error NSK_HEADER_BUILD_DIR must name the header object output directory)
endif

# Optional project settings
NSK_HEADER_CA ?= ca65
NSK_HEADER_CAFLAGS ?=

# The public fragment is staged in `bin/make` and locates framework sources
# relative to the Nesokia root. Projects never supply paths below that root.
NSK_HEADER_MK_FILE := $(abspath $(lastword $(MAKEFILE_LIST)))
NSK_HEADER_ROOT := $(abspath $(dir $(NSK_HEADER_MK_FILE))/../..)
NSK_HEADER_SOURCE_DIR := $(NSK_HEADER_ROOT)/utils/ines/header/source
NSK_HEADER_COMMON_6502_DIR := $(NSK_HEADER_ROOT)/bin/includes/6502

ifeq ($(wildcard $(NSK_HEADER_SOURCE_DIR)),)
$(error NSK header source directory is missing: $(NSK_HEADER_SOURCE_DIR))
endif

ifeq ($(wildcard $(NSK_HEADER_COMMON_6502_DIR)),)
$(error Nesokia SDK includes are missing; build nesokia-ines-header first)
endif

NSK_HEADER_CONFIG_DIR := $(abspath $(NSK_HEADER_CONFIG_DIR))
NSK_HEADER_BUILD_DIR := $(abspath $(NSK_HEADER_BUILD_DIR))
NSK_HEADER_CONFIG_INCLUDE_DIRS ?= $(NSK_HEADER_CONFIG_DIR)

NSK_HEADER_SOURCE := $(NSK_HEADER_SOURCE_DIR)/nsk_header_code.asm
NSK_HEADER_INCLUDES := $(shell find "$(NSK_HEADER_SOURCE_DIR)" -type f -name '*.inc' | sort)
NSK_HEADER_CONFIG_INCLUDES := $(shell find "$(NSK_HEADER_CONFIG_DIR)" -type f -name '*.inc' | sort)
NSK_HEADER_COMMON_INCLUDES := $(shell find "$(NSK_HEADER_COMMON_6502_DIR)" -type f -name '*.inc' | sort)
NSK_HEADER_DEPENDENCIES := \
	$(NSK_HEADER_SOURCE) \
	$(NSK_HEADER_INCLUDES) \
	$(NSK_HEADER_CONFIG_INCLUDES) \
	$(NSK_HEADER_COMMON_INCLUDES)

NSK_HEADER_OBJECTS ?= $(NSK_HEADER_BUILD_DIR)/header.o
NSK_HEADER_INCLUDE_DIRS := \
	$(NSK_HEADER_CONFIG_INCLUDE_DIRS) \
	$(NSK_HEADER_SOURCE_DIR) \
	$(NSK_HEADER_COMMON_6502_DIR)
NSK_HEADER_INCLUDE_FLAGS := $(foreach dir,$(NSK_HEADER_INCLUDE_DIRS),-I $(dir))

.PHONY: header
header: $(NSK_HEADER_OBJECTS)

$(NSK_HEADER_OBJECTS): $(NSK_HEADER_DEPENDENCIES)
	@mkdir -p "$(dir $@)"
	$(NSK_HEADER_CA) $(NSK_HEADER_CAFLAGS) $(NSK_HEADER_INCLUDE_FLAGS) -o "$@" "$(NSK_HEADER_SOURCE)"

ifeq ($(strip $(NSK_HEADER_SAVED_DEFAULT_GOAL)),)
.DEFAULT_GOAL :=
else
.DEFAULT_GOAL := $(NSK_HEADER_SAVED_DEFAULT_GOAL)
endif

endif
