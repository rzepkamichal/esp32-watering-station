# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/weekday_conversion
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/weekday_conversion -lweekday_conversion
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += weekday_conversion
COMPONENT_LDFRAGMENTS += 
component-weekday_conversion-build: component-driver-build component-esp32-build component-freertos-build
