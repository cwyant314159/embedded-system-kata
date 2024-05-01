# Compiler flags
#
# The main Makefile located in the root of this repo includes this file for
# the compiler flags passed to the toolchain during builds.
#
# This file requires the LDSCRIPT (linker script), INC_FLAGS (space separated
# list of -I/include/path), BINARY_DIR (output binary folder), and BINARY_NAME
# (compiled binary name with no extension) variables defined.
#
# Inputs:
#   INC_FLAGS
#   BINARY_DIR
#   BINARY_NAME
#
# Outputs:
#   CFLAGS
#   CXXFLAGS
#   ASFLAGS
#   LDFLAGS
#

# flags common to all compilation steps #
_COMPILE_FLAGS := -mmcu=atmega328p
_COMPILE_FLAGS += -MMD
_COMPILE_FLAGS += -MP
_COMPILE_FLAGS += -g
_COMPILE_FLAGS += -fstack-usage
_COMPILE_FLAGS += -ffunction-sections
_COMPILE_FLAGS += -fdata-sections
_COMPILE_FLAGS += -fno-common
_COMPILE_FLAGS += -fsigned-char
_COMPILE_FLAGS += -O0

# warnings #
_COMPILE_FLAGS += -Wall
_COMPILE_FLAGS += -Wextra
_COMPILE_FLAGS += -Wundef
_COMPILE_FLAGS += -Wshadow
_COMPILE_FLAGS += -Wredundant-decls
_COMPILE_FLAGS += -Wswitch-enum
_COMPILE_FLAGS += -Wno-unused-parameter
_COMPILE_FLAGS += -pedantic

# defines #
_COMPILE_FLAGS += -DF_CPU=16000000

# header path includes flags #
_COMPILE_FLAGS += $(INC_FLAGS)

# C compiler flags #
CFLAGS := $(_COMPILE_FLAGS)
CFLAGS += -Wimplicit-function-declaration
CFLAGS += -Wmissing-prototypes
CFLAGS += -Wstrict-prototypes
CFLAGS += -std=c11

# C++ compiler flags #
CXXFLAGS := $(_COMPILE_FLAGS)
CXXFLAGS += -Weffc++
CXXFLAGS += -fno-exceptions
CXXFLAGS += -fno-rtti
CXXFLAGS += -fno-use-cxa-atexit
CXXFLAGS += -std=c++11

# assembler flags #
ASFLAGS := $(_COMPILE_FLAGS) 

# linker flags #
LDFLAGS += -T$(LDSCRIPT)
LDFLAGS += -Wl,-Map="$(BINARY_DIR)/$(BINARY_NAME).map"
LDFLAGS += -Wl,-print-memory-usage
LDFLAGS += -static
LDFLAGS += -Wl,--gc-sections
