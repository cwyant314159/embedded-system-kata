set(EXE_NAME "03_hello_morse")

add_executable(
    ${EXE_NAME}
    src/main.c
)

target_link_options(${EXE_NAME} PRIVATE -Wl,-Map=${EXE_NAME}.map )

generate_artifacts(${EXE_NAME})
