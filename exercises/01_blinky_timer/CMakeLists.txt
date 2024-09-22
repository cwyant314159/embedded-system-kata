set(EXE_NAME "01_blinky_timer")
set(EXE_ALT_NAME "01_blinky_timer_alternate")

add_executable(
    ${EXE_NAME}
    src/main.c
)

target_link_options(${EXE_NAME} PRIVATE -Wl,-Map=${EXE_NAME}.map )

generate_artifacts(${EXE_NAME})

# add_executable(
#     01_blinky_timer_alternate
#     src/main_alternate.c
# )