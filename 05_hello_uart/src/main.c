#include "bsp/bsp.h"
#include "bsp/sw_timers.h"
#include "types.h"

static SwTimerHandle_t delay_timer;
static const u32_t MESSAGE_DELAY_SEC = 1u;

static void say_hello(void);

/**
 * @brief Hello, UART!
 *
 * Display "Hello, UART!" in your favorite terminal program.
 */
int main(void)
{
    /* Intialize the hardware and software modules */
    bsp_init();              /* board support (e.g. the LED) */
    sw_timer_init();         /* software timer facility */
    
    /* initialize data for the delay timer */
    delay_timer = sw_timer_acquire();
    if (SW_TIMER_NO_TIMER == delay_timer) {
       bsp_error_trap();
    }

    /* enable interrupts */
    bsp_enable_interrupts();

    /* Scheduler loop */
    while (1) {
        sw_timer_task();

        /* Print the message once we hit the timeout value. */
        if (MESSAGE_DELAY_SEC == sw_timer_sec(delay_timer)) {
            say_hello();
            sw_timer_reset(delay_timer);
        }
    }

    return 0; /* Satisfy compiler. Should never get here */
}

static void say_hello(void)
{
    bool_t success;
    const char * curr_char;
    static const char * const MESSAGE = "Hello, UART!\n\r";

    curr_char = MESSAGE;
    while ('\0' != *curr_char) {
        success = bsp_serial_write((u8_t)*curr_char);
        if (E_FALSE == success) {
            bsp_error_trap();
        }

        curr_char += 1;
    }
}