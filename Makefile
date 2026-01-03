MAKEFLAGS += --no-print-directory

UTILS_DIR := utils
UTIL_MODULES := \
  $(UTILS_DIR)/ines/header  \
  $(UTILS_DIR)/ines/inspect \
  $(UTILS_DIR)/chr/convert

.DEFAULT_GOAL := all
.PHONY: all build test clean

all:   $(addprefix   all-,$(UTIL_MODULES))
build: $(addprefix build-,$(UTIL_MODULES))
test:  $(addprefix  test-,$(UTIL_MODULES))
clean: $(addprefix clean-,$(UTIL_MODULES))

define MODULE_RULES
.PHONY: all-$(1) build-$(1) test-$(1) clean-$(1)

all-$(1):
	@$$(MAKE) -C "$(1)" all

build-$(1):
	@$$(MAKE) -C "$(1)" build

test-$(1):
	@$$(MAKE) -C "$(1)" test

clean-$(1):
	@$$(MAKE) -C "$(1)" clean
endef

$(foreach m,$(UTIL_MODULES),$(eval $(call MODULE_RULES,$(m))))