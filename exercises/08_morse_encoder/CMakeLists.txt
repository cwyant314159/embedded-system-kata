set(EXE_NAME "08_morse_encoder")

add_executable(${EXE_NAME}
    src/main.c
    src/string_encoder.c
    src/string_encoder.h
)

target_link_options(${EXE_NAME} PRIVATE -Wl,-Map=${EXE_NAME}.map )
generate_artifacts(${EXE_NAME})
target_link_libraries(${EXE_NAME} bsp morse util)
