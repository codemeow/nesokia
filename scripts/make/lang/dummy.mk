.PHONY: build

build: $(DIR_BIN)/$(PROJECT_NAME)

$(DIR_BIN)/$(PROJECT_NAME):
	@$(call print_category,Building)
	@$(call print_entry,No need to build)
