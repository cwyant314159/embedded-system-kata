cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(embedded-systems-kata
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
# Linker flags
#
add_link_options(
    -mmcu=${MCU}
    -static
    -Wl,--gc-sections
)

#
# Build flags common to C, C++, and ASM
#
set(COMMON_BUILD_FLAGS
    "-mmcu=${MCU} \
    -DF_CPU=16000000 \
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

set(CMAKE_C_FLAGS
   "${CMAKE_C_FLAGS} ${COMMON_BUILD_FLAGS} \
    -Wimplicit-function-declaration \
    -Wmissing-prototypes \
    -Wstrict-prototypes"
)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ${COMMON_BUILD_FLAGS} \
    -Weffc++ \
    -fno-exceptions \
    -fno-rtti \
    -fno-use-cxa-atexit"
)

set(ASMFLAGS
    "${COMMON_BUILD_FLAGS}"
)


#
# Add the exercise solutions to the build.
#
add_subdirectory(exercises)
