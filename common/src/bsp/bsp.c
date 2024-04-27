#include "bsp/bsp.h"

#include "bsp/reg_io.h"

#define LED_PORT        (GPIO_B)

#define LED_PIN         (5u)
#define LED_PIN_MASK    (1 << LED_PIN)

/**
 * @brief BSP initialization
 * 
 * This function is responsible for initializing low level hardware and any
 * board specific services needed by the application.
 * 
 * @note This function does not enable interrupts.
 */
void bsp_init(void)
{
    LED_PORT->DDR   = 0x00u;        /* all pins as inputs        */
    LED_PORT->PORT  = 0x00u;        /* disable internal pull-ups */
    LED_PORT->DDR   = LED_PIN_MASK; /* LED pin as output         */
    bsp_set_builtin_led(E_OFF);     /* LED is off on startup */
}

/**
 * @brief Toggle the board's builtin LED.
 */
void bsp_toggle_builtin_led(void)
{
    if ( (LED_PORT->PIN & LED_PIN_MASK) == 0x00 ) {
        bsp_set_builtin_led(E_ON);
    } else {
        bsp_set_builtin_led(E_OFF);
    }
}

/**
 * @brief Turn the board's builtin LED on or off
 * 
 * @param[in] led_state desired on/off state
 */
void bsp_set_builtin_led(on_off_t led_state)
{
    if (E_ON == led_state) {
        LED_PORT->PORT |= LED_PIN_MASK;
    } else {
        LED_PORT->PORT &= ~LED_PIN_MASK;
    }
}

/**
 * @brief Busy loop (blocking) delay
 * 
 * Using two loops this function is able to produce a human perceptable delay.
 * There has been no measurement to determine the amount of delay time. Even
 * with an iteration count 1, the delay is observable.
 * 
 * @param[in] iter number of times to sit in the spin loop.
 */
void bsp_spin_delay(size_t iter)
{
    for (volatile size_t i = iter; i != 0u; i -= 1u) {
        for (volatile size_t j = (size_t)0xFFFFu; j != 0u; j -= 1u);
    }
}

/**
 * @brief Fatal error trap
 * 
 * When the processor encounters a fatal error there isn't really anything that
 * can be done. For this project, a while loop flashing the LED is the path
 * forward. The intent is that fatal errors are programming bugs and developer
 * intervention/reset are required to remediate the situation.
 * 
 * @param[in] iter number of times to sit in the spin loop.
 */
void bsp_error_trap(void)
{
    while(1) {
        bsp_toggle_builtin_led();
        bsp_spin_delay(1);
    }
}