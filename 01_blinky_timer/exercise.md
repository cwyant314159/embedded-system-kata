# Blinky with Timer

The last exercise established a development environment and also implemented the
traditional hello world applications for embedded systems (blinky). This
exercise extends the blinky application by utilizing a hardware timer for more
precise control over the blinking frequency. Upon completion of this exercise,
you should have the following:

- processor interrupts properly configured
- a reliable time base for other applications

## Exercise

Write an application that blinks an LED once per second (500ms on/500ms off)
using one of the processor's hardware timers. The blinking should be done inside
the ISR. The main routine should only contain hardware initialization and an
empty `while(1)` loop.

### Bonus

Create an API for the hardware timer that will make it easier to port to a
different processor. The `bsp` folder is the ideal place to put this API.

