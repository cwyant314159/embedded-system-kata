#include "string_encoder.h"
#include "bsp/bsp.h"
#include "bsp/sw_timers.h"
#include "morse/task.h"
#include "types.h"

#define MORSE_TASK_INTERVAL_MSEC (100U) /* see morse_task() documentation */

/**
 * @brief Morse encoder
 *
 * Encode a string from the UART into morse code and blink it out the builtin
 * LED.
 */
int main(void)
{
    SwTimerHandle_t morse_interval_timer;

    /* Initialize the hardware and software modules */
    bsp_init();             /* board support (e.g. the LED) */
    sw_timer_init();        /* initialize the software timer facility */
    morse_task_init();      /* initialize the morse code encoder task */
    string_encoder_init();  /* initialize the string encoder processor */

    /* enable interrupts */
    bsp_enable_interrupts();

    /* initialize data for the delay timer */
    morse_interval_timer = sw_timer_acquire();
    if (SW_TIMER_NO_TIMER == morse_interval_timer) {
        bsp_error_trap();
    }

    /* Scheduler loop */
    while (1) {

        /* Constantly spin the string encoder process, so we don't miss any
           bytes. */
        string_encoder_process();

        /* Call the morse code task at the appropriate rate for morse code
           output. */
        if (MORSE_TASK_INTERVAL_MSEC == sw_timer_msec(morse_interval_timer)) {
            morse_task();
            sw_timer_reset(morse_interval_timer);
        }
    }

    return 0; /* Satisfy compiler. Should never get here */
}
