#ifndef TIMER_16BIT_H
#define TIMER_16BIT_H

#include "types.h"
#include "bsp/private/processor/reg_io.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum timer_prescaler
{
    E_TIMER_PRESCALE_0,       /* no clock source (same as pausing timer) */
    E_TIMER_PRESCALE_1,       /* CLK_io div 1 */
    E_TIMER_PRESCALE_8,       /* CLK_io div 8 */
    E_TIMER_PRESCALE_64,      /* CLK_io div 64 */
    E_TIMER_PRESCALE_256,     /* CLK_io div 256 */
    E_TIMER_PRESCALE_1024,    /* CLK_io div 1024 */
} TimerPrescaler_t;

void timer_16bit_init(Timer16BitTypeDef* p_timer);
void timer_16bit_set_prescaler(Timer16BitTypeDef* p_timer, TimerPrescaler_t prescaler);
void timer_16bit_set_ticks(Timer16BitTypeDef* p_timer, u16_t ticks);
void timer_16bit_set_callback(Timer16BitTypeDef* p_timer, IsrCallback_t cb);

#ifdef __cplusplus
}
#endif

#endif /* TIMER_16BIT_H */