#include "bsp/bsp.h"

static void echo(void);

/**
 * @brief UART echo
 *
 * Echo data from the serial port back to the host.
 */
int main(void)
{
    /* Initialize the hardware and software modules */
    bsp_init();              /* board support (e.g. the LED) */

    /* enable interrupts */
    bsp_enable_interrupts();

    /* Scheduler loop */
    while (1) {
        echo();
    }

    return 0; /* Satisfy compiler. Should never get here */
}

static void echo(void)
{
    uint8_t byte;
    if (bsp_serial_read(&byte)) {
        bsp_serial_write(byte);
        bsp_toggle_builtin_led();
    }
}