# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)-lib/components/onewire
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/onewire -lonewire
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += onewire
COMPONENT_LDFRAGMENTS += 
component-onewire-build: component-driver-build component-esp32-build component-freertos-build component-esp_idf_lib_helpers-build
