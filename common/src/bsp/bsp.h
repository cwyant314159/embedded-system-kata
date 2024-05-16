#ifndef BSP_H
#define BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

void bsp_init(void);
void bsp_enable_interrupts(void);
void bsp_toggle_builtin_led(void);
void bsp_set_builtin_led(on_off_t led_state);

bool_t bsp_serial_read(u8_t * const byte);
bool_t bsp_serial_write(u8_t byte);
bool_t bsp_serial_write_c_str(const char* c_str);

void bsp_register_timer_isr_callback(IsrCallback_t cb);
bool_t bsp_set_timer_period_uses(u16_t usec);
bool_t bsp_set_timer_period_msec(u16_t msec);
bool_t bsp_set_timer_period_sec(u16_t sec);

void bsp_spin_delay(size_t iter);
void bsp_error_trap(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_H */
