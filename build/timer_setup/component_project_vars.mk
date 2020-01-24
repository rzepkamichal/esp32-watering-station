# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/timer_setup
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/timer_setup -ltimer_setup
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += timer_setup
COMPONENT_LDFRAGMENTS += 
component-timer_setup-build: component-driver-build component-esp32-build component-freertos-build
