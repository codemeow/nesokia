common_modules_list := \
  base    \
  error   \
  io      \
  log     \
  math    \
  png     \
  strings \
  types

include $(foreach m,$(common_modules_list),$(DIR_COMMON)/$(m)/deps.mk)
