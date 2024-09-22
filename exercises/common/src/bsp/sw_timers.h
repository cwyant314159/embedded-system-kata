#ifndef SW_TIMERS_H
#define SW_TIMERS_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SW_TIMER_NO_TIMER   (NULL_PTR)

typedef struct sw_timer* SwTimerHandle_t;

void sw_timer_init(void);
void sw_timer_task(void);
SwTimerHandle_t sw_timer_acquire(void);
void sw_timer_reset(SwTimerHandle_t t);
u32_t sw_timer_sec(SwTimerHandle_t t);
u32_t sw_timer_msec(SwTimerHandle_t t);
u32_t sw_timer_usec(SwTimerHandle_t t);

#ifdef __cplusplus
}
#endif

#endif /* SW_TIMERS_H */