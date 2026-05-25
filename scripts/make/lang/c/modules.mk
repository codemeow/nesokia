include $(DIR_COMMON)/list.mk

COMMON_MODULES ?= $(common_modules_list)

define common_resolve
$(strip $(1) $(foreach d,$(common_deps_$(1)),$(call common_resolve,$(d))))
endef

COMMON_MODULES_ALL := $(foreach m,$(COMMON_MODULES),$(call common_resolve,$(m)))
COMMON_MODULES_UNKNOWN := $(filter-out $(common_modules_list),$(COMMON_MODULES_ALL))

ifneq ($(COMMON_MODULES_UNKNOWN),)
  $(error Unknown common modules: $(COMMON_MODULES_UNKNOWN))
endif

COMMON_MODULES_RESOLVED := $(sort $(COMMON_MODULES_ALL))
COMMON_LDLIBS := $(foreach m,$(COMMON_MODULES_RESOLVED),$(common_ldlibs_$(m)))

