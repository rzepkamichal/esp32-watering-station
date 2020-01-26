# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/components/valve_ctrl_logic
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/valve_ctrl_logic -lvalve_ctrl_logic
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += valve_ctrl_logic
COMPONENT_LDFRAGMENTS += 
component-valve_ctrl_logic-build: component-driver-build component-esp32-build component-freertos-build
