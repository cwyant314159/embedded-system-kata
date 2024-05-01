#include "bsp/bsp.h"
#include "types.h"


static volatile bool_t do_toggle_led;

static void bsp_timer_isr_callback(void);


/**
 * @brief Blinky using timer application
 * 
 * This blinks an LED once a second (500ms on/500ms off) using a hardware timer.
 * This solution uses Timer 1 (16-bit timer) of the ATmega328P controller.
 */
int main(void)
{
    /* Initialize the board support (e.g. the LED) */
    bsp_init();
    
    /* Set the LED's initial state to off */
    bsp_set_builtin_led(E_OFF);

    /* Since the BSP timer can only go up to 262 milliseconds, we must configure
       the timer to 250 milliseconds use a to know when to toggle the LED. The
       flag starts off as E_FALSE which will signal to the timer interrupt that
       the LED should be toggled on the next interrupt. This is done by setting
       the flag to E_TRUE. When the flag is E_TRUE, the timer interrupt will
       toggle the LED and set the flag back to E_FALSE. */
    do_toggle_led = E_FALSE;
    bsp_register_timer_isr_callback(bsp_timer_isr_callback);

    /* Enable interrupts now, so when the timer is configured and started the
       first interrupt is not missed */
    bsp_enable_interrupts();

    /* Set the timers interval so it will start. */
    bsp_set_timer_period_msec(250u);
    
    while (1) {
        /* Nothing to do */
    }

    return 0; /* Satisfy compiler. Should never get here */
}

/**
 * @brief BSP timer interrupt callback
 * 
 * This callback flips a boolean flag every 250ms (basically doubles the period
 * of the interrupt) and toggles the builtin LED.
 */
static void bsp_timer_isr_callback(void)
{
    if (E_TRUE == do_toggle_led) {
        bsp_toggle_builtin_led();
        do_toggle_led = E_FALSE;
    } else {
        do_toggle_led = E_TRUE;
    }
    
}
