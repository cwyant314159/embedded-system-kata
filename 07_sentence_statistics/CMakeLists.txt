set(EXE_NAME "07_sentence_statistics")

add_executable(
    ${EXE_NAME}
    src/main.c
    src/statistics.c
    src/statistics.h
)

target_link_options(${EXE_NAME} PRIVATE -Wl,-Map=${EXE_NAME}.map )

generate_artifacts(${EXE_NAME})