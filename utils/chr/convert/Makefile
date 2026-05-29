override PROJECT_NAME := nesokia-chr-convert

# Version data
VER_MAJOR := 0
VER_MINOR := 5
VER_BUILD := 10

# Common root directory
DIR_ROOT := ../../..

# Common modules
COMMON_MODULES := png types

# Compilation flags
CFLAGS := -DNSK_MODULE_PNG -DNSK_MODULE_COMPOSE

# Test dependencies
TEST_DEPS = $(DIR_BIN)/$(PROJECT_NAME)

include $(DIR_ROOT)/scripts/make/common.mk
include $(DIR_ROOT)/scripts/make/lang/c.mk
