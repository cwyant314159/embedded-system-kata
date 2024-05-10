# Embedded Systems Kata

A collection of kata like exercises focusing on C and embedded systems. 

The exercises build upon one another to work from absolutely nothing
(00_bringup) to an application that will parse a string from the UART
and blink it out in morse code on an LED (08_morse_encoder). Solutions provided
are implemented on an ATmega328P using the Arduino Uno (without the bootloader)
as the carrier.

> NOTE: Use of the Arduino Uno is __NOT__ mandatory. Any setup with an LED and a
> serial connection will work.

## Project Roadmap
- 08_morse_encoder
  - Application that converts newline terminated strings from the UART to
    morse code