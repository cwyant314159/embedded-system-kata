# Hello, UART!

Let's take a break from morse code for a bit and add a new peripheral to our 
toolbox. 

This exercise introduces the UART peripheral. The [Wikipedia](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter)
entry and your processor reference manual will have all the information you need
to understand the UART.

## Exercise

Write an application that uses the UART to write the following string to your
favorite terminal application once a second:

    Hello, UART!<new line>

Configure the UART with 19200 baud, 8-n-1, and no flow control. Since UART
control is hardware dependent and the exact UART used to communicate with the
host is board dependent, put your UART code in the BSP.

### Bonus

Use interrupts to transmit data to the host.
