# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(IDF_PATH)-lib/components/i2cdev
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/i2cdev -li2cdev
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += i2cdev
COMPONENT_LDFRAGMENTS += 
component-i2cdev-build: component-driver-build component-freertos-build
