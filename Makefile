#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

PROJECT_NAME := blink
EXTRA_COMPONENT_DIRS := /home/musr/dev/esp/esp-idf-lib/components

include $(IDF_PATH)/make/project.mk

