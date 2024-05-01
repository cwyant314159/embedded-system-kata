# Exercise specific flags
#
# The main Makefile located in the root of this repo includes this file for 
# exercise source files and build artifact names.
#
# This file requires the OBJ_ROOT_DIR (root folder for .o files) and
# BIN_ROOT_DIR (root folder for compiled executables) variables defined.
#
# Inputs:
#   BIN_ROOT_DIR
#   OBJ_ROOT_DIR
#
# Outputs:
#   BINARY_NAME
#   BINARY_DIR
#   OBJ_DIR
#   INC_FLAGS
#   SRC_FILES
#   LDSCRIPT

# executable binary name #
BINARY_NAME := 00_bringup

# directory for the final compiled binary #
BINARY_DIR := $(BIN_ROOT_DIR)/$(BINARY_NAME)

# directory for compiled objects #
OBJ_DIR := $(OBJ_ROOT_DIR)/$(BINARY_NAME)

# include directories used by the exercise #
_INC_DIRS := common/src
_INC_DIRS += $(BINARY_NAME)/src

INC_FLAGS := $(foreach dir, $(_INC_DIRS), $(addprefix -I, $(dir)))

# directories containing source files for the exercise (recursively searched)
_SRC_DIRS := common/src
_SRC_DIRS += $(BINARY_NAME)/src

# recursively search the _SRC_DIRS for .c, .cpp, and .s files
SRC_FILES := $(foreach dir, $(_SRC_DIRS), $(shell find $(dir) -type f -name '*.cpp'))
SRC_FILES += $(foreach dir, $(_SRC_DIRS), $(shell find $(dir) -type f -name '*.c'))
SRC_FILES += $(foreach dir, $(_SRC_DIRS), $(shell find $(dir) -type f -name '*.s'))

# application linker script #
LDSCRIPT := common/linker/avr.ld