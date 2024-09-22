#
# Add the common directory and individual exercise directories to the build.
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