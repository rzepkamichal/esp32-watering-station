# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)-lib/components/hx711
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/hx711 -lhx711
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += hx711
COMPONENT_LDFRAGMENTS += 
component-hx711-build: component-driver-build component-esp32-build component-freertos-build
