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
# Add subordinate CMakeList.txt files
#
add_subdirectory(common)
add_subdirectory(01_blinky_timer)
add_subdirectory(02_sos)
add_subdirectory(03_hello_morse)
add_subdirectory(04_morse_c_str)
add_subdirectory(05_hello_uart)
add_subdirectory(06_uart_echo)
add_subdirectory(07_sentence_statistics)
add_subdirectory(08_morse_encoder)


#
# Link all the applications and libraries together
#
target_link_libraries(01_blinky_timer bsp)
target_link_libraries(02_sos bsp)
target_link_libraries(03_hello_morse bsp morse util)
target_link_libraries(04_morse_c_str bsp morse util)
target_link_libraries(05_hello_uart bsp)
target_link_libraries(06_uart_echo bsp)
target_link_libraries(07_sentence_statistics bsp util)
target_link_libraries(08_morse_encoder bsp morse util)

