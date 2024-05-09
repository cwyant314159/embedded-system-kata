# UART Echo

By now you should have the outbound (transmit) UART traffic working. This
exercise will get the inbound (receiver) side working.

## Exercise

Write an application that echoes the data received by the UART's receiver to 
the terminal application (basically a loop back).

It might be helpful to toggle your LED every time you receive a character and
successfully transmit it.

### Bonus

Use interrupts on both the transmit and receive side.
