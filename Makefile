MAKEFLAGS += --no-print-directory

UTILS_DIR := utils
UTIL_MODULES := \
  $(UTILS_DIR)/ines/header  \
  $(UTILS_DIR)/ines/frame   \
  $(UTILS_DIR)/ines/inspect \
  $(UTILS_DIR)/chr/colors   \
  $(UTILS_DIR)/chr/convert

EXAMPLES_DIR := examples
EXAMPLE_MODULES := \
  $(EXAMPLES_DIR)/platformer

MODULES := $(UTIL_MODULES) $(EXAMPLE_MODULES)

.DEFAULT_GOAL := all
.PHONY: all build test test-valgrind clean

all:   $(addprefix   all-,$(MODULES))
build: $(addprefix build-,$(MODULES))
test:  $(addprefix  test-,$(MODULES))
test-valgrind:  $(addprefix test-valgrind-,$(MODULES))
clean: $(addprefix clean-,$(MODULES))

define MODULE_RULES
.PHONY: all-$(1) build-$(1) test-$(1) test-valgrind-$(1) clean-$(1)

all-$(1):
	@$$(MAKE) -C "$(1)" all

build-$(1):
	@$$(MAKE) -C "$(1)" build

test-$(1):
	@$$(MAKE) -C "$(1)" test

test-valgrind-$(1):
	@$$(MAKE) -C "$(1)" test-valgrind

clean-$(1):
	@$$(MAKE) -C "$(1)" clean
endef

$(foreach m,$(MODULES),$(eval $(call MODULE_RULES,$(m))))

all-$(EXAMPLES_DIR)/platformer: all-$(UTILS_DIR)/ines/header all-$(UTILS_DIR)/ines/frame all-$(UTILS_DIR)/chr/convert
build-$(EXAMPLES_DIR)/platformer: build-$(UTILS_DIR)/ines/header build-$(UTILS_DIR)/ines/frame build-$(UTILS_DIR)/chr/convert
test-$(EXAMPLES_DIR)/platformer: test-$(UTILS_DIR)/ines/header test-$(UTILS_DIR)/ines/frame test-$(UTILS_DIR)/chr/convert
test-valgrind-$(EXAMPLES_DIR)/platformer: test-valgrind-$(UTILS_DIR)/ines/header test-valgrind-$(UTILS_DIR)/ines/frame test-valgrind-$(UTILS_DIR)/chr/convert
