#include "bsp/bsp.h"

#include <avr/interrupt.h>
#include "bsp/private/processor/reg_io.h"
#include "bsp/private/timer/timer.h"
#include "bsp/private/uart/uart.h"

#define LED_PORT        (GPIO_B)

#define LED_PIN         (5u)
#define LED_PIN_MASK    (1 << LED_PIN)

#define BSP_TIMER       (TIM1)
#define MAX_USEC        (32767u)
#define MAX_MSEC        (262u)
#define MAX_SEC         (4u)


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
    LED_PORT->DDR   = 0x00u;        /* all pins as inputs              */
    LED_PORT->PORT  = 0x00u;        /* disable internal pull-ups       */
    LED_PORT->DDR   = LED_PIN_MASK; /* LED pin as output               */
    bsp_set_builtin_led(E_OFF);     /* LED is off on startup           */
    timer_16bit_init(BSP_TIMER);    /* initialize (and stop) the timer */
    uart_init();
}

/**
 * @brief Enable interrupts on the processor
 *
 * To insulate the application from details about the processor, the global
 * interrupt enable is put behind this interface function.
 */
void bsp_enable_interrupts(void)
{
    sei(); /* interrupt enable from the AVR header */
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
 * @brief Read a byte from the serial driver
 *
 * @param[out] byte output read from serial port
 *
 * @retval E_TRUE  - successfully read byte
 * @retval E_FALSE - unable to read byte
 */
bool_t bsp_serial_read(u8_t * const byte)
{
    bool_t result;

    result = E_FALSE;
    if ((NULL_PTR != byte) && (E_TRUE == uart_data_available())) {
        *byte = uart_read();
        result = E_TRUE;
    }

    return result;
}

/**
 * @brief Write a byte to the serial driver
 *
 * @param[in] byte input to write to UART
 *
 * @retval E_TRUE  - successfully write byte
 * @retval E_FALSE - unable to write byte
 */
bool_t bsp_serial_write(u8_t byte)
{
    return uart_write(byte);
}

/**
 * @brief Write a C-style string out the serial port.
 *
 * @param[in] c_str null terminated string to output over serial
 *
 * @retval E_TRUE  - successfully wrote string to serial driver
 * @retval E_FALSE - serial driver encountered an error during write
 */
bool_t bsp_serial_write_c_str(const char* c_str)
{
    const char *p_c;
    bool_t      result;

    p_c    = c_str;
    result = E_TRUE;

    while('\0' != *p_c) {
        result = bsp_serial_write(*p_c);
        p_c += 1;

        /* If an error occurs, don't bother sending the rest of the string. */
        if (E_FALSE == result) {
            break;
        }
    }

    return result;
}

/**
 * @brief Set the BSP's timer interrupt callback.
 *
 * @param[in] cb user supplied callback to handle BSP timer interrupts.
 */
void bsp_register_timer_isr_callback(IsrCallback_t cb)
{
    timer_16bit_set_callback(BSP_TIMER, cb);
}

/**
 * @brief Set the BSP timer's period in microseconds and start running.
 *
 * @note The timer can only handle a microsecond range of 0 (off) to 32767
 * microseconds
 *
 * @param[in] usec timer interval in microseconds
 *
 * @retval E_TRUE  - successfully configured the timer
 * @retval E_FALSE - failed to set timer's interval
 */
bool_t bsp_set_timer_period_uses(u16_t usec)
{
    /*
     * To have microsecond accuracy, the timer must be configured with a
     * prescaler of 8 which means that each microsecond is 2 ticks.
     */
    static const u16_t PRESCALED_TICKS_PER_USEC = 2u;
    static const TimerPrescaler_t PRESCALER = E_TIMER_PRESCALE_8;

    bool_t result;
    u16_t ticks;

    /* assume configuration will fail */
    result = E_FALSE;

    /*
     * Changing the period of the timer requires resetting it. Call the timer's
     * initialization function to stop the timer and put it in a known starting
     * state.
     */
    timer_16bit_init(BSP_TIMER);

    /* Convert the duration into timer ticks. Remember that we need to subtract
       1 from our calculated tick value to compensate for the tick back to 0. */
    ticks = (usec * PRESCALED_TICKS_PER_USEC) - 1U;

    if (MAX_USEC >= usec) {
        timer_16bit_set_ticks(BSP_TIMER, ticks);
        timer_16bit_set_prescaler(BSP_TIMER, PRESCALER);
        result = E_TRUE;
    }

    return result;
}

/**
 * @brief Set the BSP timer's period in milliseconds and start running.
 *
 * @note The timer can only handle a millisecond range of 0 (off) to 262
 * milliseconds
 *
 * @param[in] msec timer interval in milliseconds
 *
 * @retval E_TRUE  - successfully configured the timer
 * @retval E_FALSE - failed to set timer's interval
 */
bool_t bsp_set_timer_period_msec(u16_t msec)
{
    /*
     * To have millisecond accuracy, the timer must be configured with a
     * prescaler of 64 which means that each microsecond is 250 ticks.
     */
    static const u16_t PRESCALED_TICKS_PER_MSEC = 250u;
    static const TimerPrescaler_t PRESCALER = E_TIMER_PRESCALE_64;

    bool_t result;
    u16_t ticks;

    /* assume configuration will fail */
    result = E_FALSE;

    /*
     * Changing the period of the timer requires resetting it. Call the timer's
     * initialization function to stop the timer and put it in a known starting
     * state.
     */
    timer_16bit_init(BSP_TIMER);

    /* Convert the duration into timer ticks. Remember that we need to subtract
       1 from our calculated tick value to compensate for the tick back to 0. */
    ticks = (msec * PRESCALED_TICKS_PER_MSEC) - 1U;

    if (MAX_MSEC >= msec) {
        timer_16bit_set_ticks(BSP_TIMER, ticks);
        timer_16bit_set_prescaler(BSP_TIMER, PRESCALER);
        result = E_TRUE;
    }

    return result;
}

/**
 * @brief Set the BSP timer's period in seconds and start running.
 *
 * @note The timer can only handle a second range of 0 (off) to 4 seconds
 *
 * @param[in] sec timer interval in seconds
 *
 * @retval E_TRUE  - successfully configured the timer
 * @retval E_FALSE - failed to set timer's interval
 */
bool_t bsp_set_timer_period_sec(u16_t sec)
{
    /*
     * To have second accuracy, the timer must be configured with a prescaler
     * of 1024 which means that each microsecond is 15625 ticks.
     */
    static const u16_t PRESCALED_TICKS_PER_SEC = 15625u;
    static const TimerPrescaler_t PRESCALER = E_TIMER_PRESCALE_1024;

    bool_t result;
    u16_t ticks;

    /* assume configuration will fail */
    result = E_FALSE;

    /*
     * Changing the period of the timer requires resetting it. Call the timer's
     * initialization function to stop the timer and put it in a known starting
     * state.
     */
    timer_16bit_init(BSP_TIMER);

    /* Convert the duration into timer ticks. Remember that we need to subtract
       1 from our calculated tick value to compensate for the tick back to 0. */
    ticks = (sec * PRESCALED_TICKS_PER_SEC) - 1U;

    if (MAX_SEC >= sec) {

        /*
        * To have second accuracy, the timer must be configured with a prescaler
        * of 1024 which means that each microsecond is 15625 ticks.
        *
        * Remember that we need to subtract 1 from our calculated tick value to
        * compensate for the tick back to 0.
        */
        timer_16bit_set_ticks(BSP_TIMER, ticks);
        timer_16bit_set_prescaler(BSP_TIMER, PRESCALER);
        result = E_TRUE;
    }

    return result;
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