#ifndef BSP_H
#define BSP_H

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ISR callback function pointer type.
 *
 * Maps to function with the signature void my_callback(void)
 */
typedef void (*IsrCallback_t)(void);

/*
 * On/Off enumerator
 */
typedef enum on_off_type
{
    E_ON  = 1,
    E_OFF = 0,
} on_off_t;

void bsp_init(void);
void bsp_enable_interrupts(void);
void bsp_toggle_builtin_led(void);
void bsp_set_builtin_led(on_off_t led_state);

int bsp_serial_read(uint8_t * const byte);
int bsp_serial_write(uint8_t byte);
int bsp_serial_write_c_str(const char* c_str);

void bsp_register_timer_isr_callback(IsrCallback_t cb);
int bsp_set_timer_period_usec(uint16_t usec);
int bsp_set_timer_period_msec(uint16_t msec);
int bsp_set_timer_period_sec(uint16_t sec);

void bsp_spin_delay(size_t iter);
void bsp_error_trap(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_H */
