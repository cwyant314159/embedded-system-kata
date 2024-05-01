# Embedded Systems Kata

A collection of kata like exercies focusing on C and embedded systems. 

The exercises build upon one another to work from absolutely nothing
(00_bringup) to an application that will convert a string from the UART
and blink it out in morse code on an LED (08_morse_encoder). Solutions provided
are implemented on an ATmega328P using the Arduino Uno (without the bootloader)
as the carrier.

> NOTE: Use of the Arduino Uno is __NOT__ mandatory. Any setup with an LED and a
> serial connection to a host computer will work.



## TODO

- 03_morse_code_hello_world
  - refactor sos project to do hello world
- 04_morse_code_c_cstr
  - refactor previous project to use any c string
- 05_uart_hello_world
  - print hello world using the UART (bonus: use interrupts)
- 06_uart_echo
  - UART echo chars from host back to terminal (bonus: use interrupts)
- 07_uart_sentence_statistics
  - App to count the number of letters, white space chars, and punctionation in 
    a new line separated string.
- 08_morse_encoder
  - Application that converts newline terminated strings from the UART to
    morse code