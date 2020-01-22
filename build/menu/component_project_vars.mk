# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/menu
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/menu -lmenu
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += menu
COMPONENT_LDFRAGMENTS += 
component-menu-build: component-driver-build component-esp32-build component-freertos-build
