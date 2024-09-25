# All CMake output will be under this build directory. Different build types
# will have its own subdirectory beneath this top level build directory.
CMAKE_BUILD_ROOT       := build
DEBUG_BUILD_ROOT       := $(CMAKE_BUILD_ROOT)/debug
RELEASE_BUILD_ROOT     := $(CMAKE_BUILD_ROOT)/release
MIN_RELEASE_BUILD_ROOT := $(CMAKE_BUILD_ROOT)/min-release

# Use the AVR toolchain file during CMake invocations
TOOLCHAIN := avr-gcc-toolchain.cmake

# CppCheck flags
CPPCHECK_FLAGS := --enable=all
CPPCHECK_FLAGS += --enable=style
CPPCHECK_FLAGS += --platform=avr8
CPPCHECK_FLAGS += --std=c11
CPPCHECK_FLAGS += --std=c++17
CPPCHECK_FLAGS += --suppress=missingIncludeSystem
CPPCHECK_FLAGS += --suppress=checkersReport
CPPCHECK_FLAGS += --suppressions-list=cppcheck-suppressions.txt

# The coding style dictates that all variables be declared at the top of
# every function. This causes CppCheck to throw "variable scope can be
# reduced" warnings.
CPPCHECK_FLAGS += --suppress=variableScope:*

all: debug release min-release

.PHONY: debug release min-release clean

#
# Debug Build
#
debug:
	@cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) \
		-S. \
		-B$(DEBUG_BUILD_ROOT)


	@cmake --build $(DEBUG_BUILD_ROOT) -j2

#
# Release Build
#
release:
	@cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) \
		-S. \
		-B$(RELEASE_BUILD_ROOT)

	@cmake --build $(RELEASE_BUILD_ROOT) -j2

#
# Minimum Size Release Build
#
min-release:
	@cmake \
		-DCMAKE_BUILD_TYPE=MinSizeRel \
		-DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) \
		-S. \
		-B$(MIN_RELEASE_BUILD_ROOT)

	@cmake --build $(MIN_RELEASE_BUILD_ROOT) -j2

#
# CppCheck targets for all the build types
#
cppcheck-debug: debug
	@cppcheck --project=$(DEBUG_BUILD_ROOT)/compile_commands.json $(CPPCHECK_FLAGS)

cppcheck-release: release
	@cppcheck --project=$(RELEASE_BUILD_ROOT)/compile_commands.json $(CPPCHECK_FLAGS)

cppcheck-min-release: min-release
	@cppcheck --project=$(MIN_RELEASE_BUILD_ROOT)/compile_commands.json $(CPPCHECK_FLAGS)

#
# Remove all CMake generated files
#
clean:
	rm -rfv $(CMAKE_BUILD_ROOT)