#include "bsp/private/timer/timer.h"

#include <avr/interrupt.h>

#include "bsp/private/processor/reg_io.h"
#include "types.h"

/* Clock select bit masks for timer control register */
#define CS_0    (1u << 0u)
#define CS_1    (1u << 1u)
#define CS_2    (1u << 2u)
#define CS_MASK (CS_2 | CS_1 | CS_0)

/* Timer mode bit mask for timer control register */
#define WGM_0   (1u << 0u)  /* TCCRA */
#define WGM_1   (1u << 1u)  /* TCCRA */
#define WGM_2   (1u << 3u)  /* TCCRB */
#define WGM_3   (1u << 4u)  /* TCCRB */

static volatile IsrCallback_t timer0_callback = NULL_PTR;
static volatile IsrCallback_t timer1_callback = NULL_PTR;
static volatile IsrCallback_t timer2_callback = NULL_PTR;

static TimerIrqRegTypeDef* get_irq_io(void* p_timer);
static void set_irq_callback(void* p_timer, IsrCallback_t cb);

void timer_16bit_init(Timer16BitTypeDef* p_timer)
{
    TimerIrqRegTypeDef *p_irq_io;

    /* Since this is an internal function (and it is super small), we are going
       to skip error checking. It's going to work... I promise. */
    p_irq_io = get_irq_io(p_timer);

    if (TIM1 == p_timer) {
        /*
        * Just to be safe the timer is stopped and interrupts are disabled before
        * initialization.
        */
        timer_16bit_set_prescaler(p_timer, E_TIMER_PRESCALE_0);
        p_irq_io->TIMSK = 0x0;

        /*
        * Configure the timer for Clear Timer on Compare Match (CTC) mode and set
        * the prescaler to no clock source
        *
        * NOTE: CTC Mode WGM[3:0] = 0100;
        */
        p_timer->TCCRA = 0;     /* low bits for mode */
        p_timer->TCCRB = WGM_2; /* upper bits for mode and the prescaler */

        /*
        * Set the timer's counter to 0.
        */
        p_timer->TCNT = 0;

        /*
        * To prevent a spurious interrupt from triggering caused by a 0 in the
        * Output Compare Register A, the register is preloaded with the maximum
        * possible value (0xFF). The application can update this value with the
        * tick setting function.
        */
        p_timer->OCRA = 0xFFFF;

        /*
        * Enable the Output Compare Match A interrupt. All other interrupts are
        * disabled.
        */
        p_irq_io->TIMSK = (1 << OCIE1A);
    }
}


void timer_16bit_set_prescaler(Timer16BitTypeDef* p_timer, TimerPrescaler_t prescaler)
{
    /*
     * NOTE: Setting the prescaler to anything other than 0 starts the timer
     *       running.
     */
    u8_t prescaler_bits;

    switch(prescaler)
    {
        case E_TIMER_PRESCALE_0   : prescaler_bits = 0x00;               break;
        case E_TIMER_PRESCALE_1   : prescaler_bits =               CS_0; break;
        case E_TIMER_PRESCALE_8   : prescaler_bits =        CS_1;        break;
        case E_TIMER_PRESCALE_64  : prescaler_bits =        CS_1 | CS_0; break;
        case E_TIMER_PRESCALE_256 : prescaler_bits = CS_2;               break;
        case E_TIMER_PRESCALE_1024: prescaler_bits = CS_2 |        CS_0; break;
        default:                    prescaler_bits = 0x00;               break;
    }

    if (TIM1 == p_timer) {
        /*
         * The old prescaler bits must be masked out before the new bits can be
         * OR'ed into the register.
         */
        p_timer->TCCRB = (p_timer->TCCRB & ~CS_MASK) | prescaler_bits;
    }
}


void timer_16bit_set_ticks(Timer16BitTypeDef* p_timer, u16_t ticks)
{
    if (TIM1 == p_timer) {
        /*
         * Callers must ensure that they pass the right tick value to this
         * function. Remember if you want 2 timer ticks before the interrupt
         * fires, the value 1 is passed to this function.
         */
        p_timer->OCRA = ticks;
    }
}


void timer_16bit_set_callback(Timer16BitTypeDef* p_timer, IsrCallback_t cb)
{
    /* The low level function will handle the error case for us. There is
       nothing we can do if the programmer fails to provide the right timer */
    set_irq_callback(p_timer, cb);
}


static TimerIrqRegTypeDef* get_irq_io(void* p_timer)
{
    TimerIrqRegTypeDef* p_irq;

    /* It's ugly but it works... */
    if ((void*)TIM0 == p_timer) {
        p_irq = TIM0_IRQ;
    } else if((void*)TIM1 == p_timer) {
        p_irq = TIM1_IRQ;
    } else if((void*)TIM2 == p_timer) {
        p_irq = TIM2_IRQ;
    } else {
        p_irq = NULL_PTR;
    }

    return p_irq;
}


static void set_irq_callback(void* p_timer, IsrCallback_t cb)
{
    /* It's ugly but it works... */
    if ((void*)TIM0 == p_timer) {
        timer0_callback = cb;
    } else if((void*)TIM1 == p_timer) {
        timer1_callback = cb;
    } else if((void*)TIM2 == p_timer) {
        timer2_callback = cb;
    } else {
        /* Nothing to do for invalid timers */
    }
}

/*
 * INTERRUPT SERVICE ROUTINES
 */
ISR(TIMER0_COMPA_vect)
{
    if (NULL_PTR != timer0_callback) {
        timer0_callback();
    }
}

ISR(TIMER1_COMPA_vect)
{
    if (NULL_PTR != timer1_callback) {
        timer1_callback();
    }
}

ISR(TIMER2_COMPA_vect)
{
    if (NULL_PTR != timer2_callback) {
        timer2_callback();
    }
}