override PROJECT_NAME := nesokia-ines-header

# Version data
VER_MAJOR := 0
VER_MINOR := 4
VER_BUILD := 1

# Common root directory
DIR_ROOT := ../../..

# Test dependencies
TEST_DEPS = $(DIR_BIN)/nesokia-ines-inspect

include $(DIR_ROOT)/scripts/make/common.mk

$(DIR_BIN)/nesokia-ines-inspect:
	@$(MAKE) -C "$(DIR_ROOT)/utils/ines/inspect" build

include $(DIR_ROOT)/scripts/make/lang/dummy.mk

