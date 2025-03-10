set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR AVR)

set(TOOLCHAIN_PREFIX avr-)

set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc -xassembler-with-cpp)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}gcc-ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)

set(CMAKE_C_FLAGS_DEBUG_INIT "-g3 -Og -DDEBUG")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG_INIT}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE_INIT "-O3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE_INIT}" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_MINSIZEREL_INIT "-Os")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL_INIT}" CACHE STRING "" FORCE)

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

function(generate_artifacts exe_name)
    add_custom_command(TARGET ${exe_name} POST_BUILD
        COMMAND
            ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${exe_name}> ${exe_name}.hex
        COMMAND
            ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${exe_name}> ${exe_name}.bin
        COMMAND
            ${CMAKE_OBJDUMP} -rdCSw $<TARGET_FILE:${exe_name}> > ${exe_name}.lss
        COMMAND
            ${CMAKE_SIZE} -A -t $<TARGET_FILE:${exe_name}> > ${exe_name}.size.txt
    )
endfunction()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)