cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    embedded-systems-kata
    VERSION
        1.0
    DESCRIPTION
        "A collection of kata like exercises focusing on C and embedded systems."
    LANGUAGES
        CXX
        C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Target AVR Microcontroller
#
set(MCU atmega328p)

#
# Build flags common to C, C++, and ASM
#
add_compile_definitions(F_CPU=16000000)

set(COMMON_BUILD_FLAGS
    "-mmcu=${MCU} \
    -fstack-usage \
    -ffunction-sections \
    -fdata-sections \
    -fno-common \
    -fsigned-char \
    -Wall \
    -Wextra \
    -Wundef \
    -Wshadow \
    -Wredundant-decls \
    -Wswitch-enum \
    -Wno-unused-parameter \
    -pedantic"
)

#
# C specific compiler flags
#
set(CMAKE_C_FLAGS
   "${CMAKE_C_FLAGS} ${COMMON_BUILD_FLAGS} \
    -Wimplicit-function-declaration \
    -Wmissing-prototypes \
    -Wstrict-prototypes"
)

#
# C++ specific compiler flags
#
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ${COMMON_BUILD_FLAGS} \
    -Weffc++ \
    -fno-exceptions \
    -fno-rtti \
    -fno-use-cxa-atexit"
)

#
# ASM specific compiler flags
#
set(ASMFLAGS "${COMMON_BUILD_FLAGS}")


#
# Linker flags
#
add_link_options(-mmcu=${MCU} -static -Wl,--gc-sections)

#
# Add the exercise solutions to the build.
#
add_subdirectory(exercises)
