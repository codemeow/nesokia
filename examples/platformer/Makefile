override PROJECT_NAME := examples/platformer.nes

# Common root directory
DIR_ROOT := ../..

ifneq ($(filter undefined default,$(origin CA)),)
CA := ca65
endif

NESOKIA_DIR ?= $(DIR_ROOT)
NSK_HEADER_MK := $(NESOKIA_DIR)/bin/make/nsk_header.mk

NSK_HEADER_SDK_REQUIRED := $(if $(MAKECMDGOALS),$(filter-out clean,$(MAKECMDGOALS)),1)
ifneq ($(strip $(NSK_HEADER_SDK_REQUIRED)),)
ifeq ($(wildcard $(NSK_HEADER_MK)),)
$(error Nesokia header SDK is missing; run make nesokia-ines-header build)
endif

NSK_HEADER_CONFIG_DIR := $(CURDIR)
NSK_HEADER_BUILD_DIR := $(DIR_ROOT)/build/$(PROJECT_NAME)
NSK_HEADER_CA := $(CA)
NSK_HEADER_CAFLAGS := -g
include $(NSK_HEADER_MK)

ASM_EXTRA_OBJECTS += $(NSK_HEADER_OBJECTS)
endif

# CHR ROM banks
CHR_BANKS := CHR0 CHR1 CHR2 CHR3

CHR_BANK_CHR0_PNG := chrrom/png/bank-0.png
CHR_BANK_CHR0_CFG := chrrom/png/bank-0.cfg
CHR_BANK_CHR0_LEFT = $(CHR_OUTPUT_DIR)/bank-0.0000.chr
CHR_BANK_CHR0_RIGHT = $(CHR_OUTPUT_DIR)/bank-0.1000.chr
CHR_BANK_CHR0_PAL_BACK = $(PAL_OUTPUT_DIR)/back.pal
CHR_BANK_CHR0_PAL_SPRITES = $(PAL_OUTPUT_DIR)/sprites.pal

CHR_BANK_CHR1_PNG := chrrom/png/bank-1.png
CHR_BANK_CHR1_CFG := chrrom/png/bank-1.cfg
CHR_BANK_CHR1_LEFT = $(CHR_OUTPUT_DIR)/bank-1.0000.chr
CHR_BANK_CHR1_RIGHT = $(CHR_OUTPUT_DIR)/bank-1.1000.chr

CHR_BANK_CHR2_PNG := chrrom/png/bank-2.png
CHR_BANK_CHR2_CFG := chrrom/png/bank-2.cfg
CHR_BANK_CHR2_LEFT = $(CHR_OUTPUT_DIR)/bank-2.0000.chr
CHR_BANK_CHR2_RIGHT = $(CHR_OUTPUT_DIR)/bank-2.1000.chr

CHR_BANK_CHR3_PNG := chrrom/png/bank-3.png
CHR_BANK_CHR3_CFG := chrrom/png/bank-3.cfg
CHR_BANK_CHR3_LEFT = $(CHR_OUTPUT_DIR)/bank-3.0000.chr
CHR_BANK_CHR3_RIGHT = $(CHR_OUTPUT_DIR)/bank-3.1000.chr

include $(DIR_ROOT)/scripts/make/common.mk
include $(DIR_ROOT)/scripts/make/asm.mk
