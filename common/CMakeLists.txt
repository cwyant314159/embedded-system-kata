#
# BSP Library
#
add_library(
    bsp
    STATIC
    src/bsp/bsp.c
    src/bsp/private/timer/timer.c
    src/bsp/private/uart/uart.c
    src/bsp/sw_timers.c
)

target_include_directories(
    bsp
    PUBLIC
    src
)

#
# Utility Library
#
add_library(
    util
    STATIC
    src/utils/ascii_char.c
    src/utils/bytes.c
)

target_include_directories(
    util
    PUBLIC
    src
)

#
# Morse Library
#
add_library(
    morse
    STATIC
    src/morse/task.c
    src/morse/private/alphabet.c
)

target_include_directories(
    morse
    PUBLIC
    src
)