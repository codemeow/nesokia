# Build directory for object files
DIR_BUILD := $(DIR_ROOT)/build/$(PROJECT_NAME)
# Output binary directory
DIR_BIN := $(DIR_ROOT)/bin

# Coloring and output
COLOR_TTY := $(shell test -t 2 && echo 1 || echo 0)

ifeq ($(and $(filter 1,$(COLOR_TTY)),$(filter-out dumb,$(TERM)),$(if $(NO_COLOR),,1)),1)
  ESC := \033
  COLOR_RESET := $(ESC)[0m
  COLOR_GREEN := $(ESC)[32m
  COLOR_WHITE := $(ESC)[1;1m
else
  COLOR_RESET :=
  COLOR_GREEN :=
endif

MSG_OK := [$(COLOR_GREEN)OK$(COLOR_RESET)]

define print_project
printf "\n%b%s%b\n" "    $(COLOR_WHITE)" "==== $(1) ====" "$(COLOR_RESET)"
endef

define print_category
printf "%b%s%b\n" "$(COLOR_WHITE)" "$(1)" "$(COLOR_RESET)"
endef

define print_entry
printf "%b    %s\n" "$(MSG_OK)" "$(1)"
endef

# Default target
.DEFAULT_GOAL := all

# Targets
.PHONY: all                             test      clean
.PHONY: $(DIR_BIN)/$(PROJECT_NAME)-pre  test-pre  clean-pre
.PHONY:                                 test-post clean-post

all: $(DIR_BIN)/$(PROJECT_NAME)-pre $(DIR_BIN)/$(PROJECT_NAME)
clean: clean-pre clean-post
test: test-pre test-post

.PHONY: $(DIR_BIN)/$(PROJECT_NAME)-pre
$(DIR_BIN)/$(PROJECT_NAME)-pre:
	@$(call print_project,$(PROJECT_NAME))

# Clean build artifacts
RM := rm -rf

clean-pre: $(DIR_BIN)/$(PROJECT_NAME)-pre
	@$(call print_category,Cleaning)

CLEAN_ENTRIES := $(DIR_BUILD) $(DIR_BIN)/$(PROJECT_NAME)

clean-post:
	@$(foreach p,$(CLEAN_ENTRIES), \
		$(RM) "$(p)" &&            \
		$(call print_entry,$(p));  \
	)

# Testing
define require-tool
	command -v $(1) >/dev/null 2>&1 || { \
		echo "ERROR: required tool '$(1)' not found" >&2; exit 127; \
	}
endef

PYTHON ?= python3
TEST_SCRIPT := tests/test.py

test-pre: $(DIR_BIN)/$(PROJECT_NAME)-pre
	@$(call print_category,Testing)

test-post:
	@if [ -f "$(TEST_SCRIPT)" ]; then \
	    $(call require-tool,$(PYTHON)); \
	    $(PYTHON) "$(TEST_SCRIPT)"; \
	else \
	    echo "    - Nothing to test"; \
	fi