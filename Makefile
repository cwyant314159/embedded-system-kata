# All CMake output will be under this build directory. Different build types
# will have its own subdirectory beneath this top level build directory.
CMAKE_BUILD_ROOT=build
DEBUG_BUILD_ROOT=$(CMAKE_BUILD_ROOT)/debug
RELEASE_BUILD_ROOT=$(CMAKE_BUILD_ROOT)/release
MIN_RELEASE_BUILD_ROOT=$(CMAKE_BUILD_ROOT)/min-release

# Use the AVR toolchain file during CMake invocations
TOOLCHAIN=avr-gcc-toolchain.cmake

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


	@cmake --build $(DEBUG_BUILD_ROOT)

#
# Release Build
#
release:
	@cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) \
		-S. \
		-B$(RELEASE_BUILD_ROOT)

	@cmake --build $(RELEASE_BUILD_ROOT)

#
# Minimum Size Release Build
#
min-release:
	@cmake \
		-DCMAKE_BUILD_TYPE=MinSizeRel \
		-DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN) \
		-S. \
		-B$(MIN_RELEASE_BUILD_ROOT)

	@cmake --build $(MIN_RELEASE_BUILD_ROOT)

#
# Remove all CMake generated files
#
clean:
	rm -rfv $(CMAKE_BUILD_ROOT)