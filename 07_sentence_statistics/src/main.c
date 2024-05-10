#include "statistics.h"
#include "bsp/bsp.h"
#include "bsp/sw_timers.h"
#include "types.h"

#define TOGGLE_PERIOD_MSEC (500U)

/**
 * @brief Sentence statistics
 *
 * Dump the following information to the UART when a new line is received:
 *
 * - number of letters
 * - number of vowels
 * - number of digits
 * - number of whitespace characters (including the new line but not NULL)
 * - number of punctuation characters
 */
int main(void)
{
    SwTimerHandle_t delay_timer;

    /* Intialize the hardware and software modules */
    bsp_init();         /* board support (e.g. the LED) */
    sw_timer_init();    /* software timer facility */
    statistics_init();  /* initialize the statistics module */

    /* enable interrupts */
    bsp_enable_interrupts();

    /* initialize data for the delay timer */
    delay_timer = sw_timer_acquire();
    if (SW_TIMER_NO_TIMER == delay_timer) {
        bsp_error_trap();
    }

    /* Scheduler loop */
    while (1) {
        /* Print the message once we hit the timeout value. */
        if (TOGGLE_PERIOD_MSEC == sw_timer_msec(delay_timer)) {
            bsp_toggle_builtin_led();
            sw_timer_reset(delay_timer);
        }

        statistics_task();
    }

    return 0; /* Satisfy compiler. Should never get here */
}
