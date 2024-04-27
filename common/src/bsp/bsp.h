#ifndef BSP_H
#define BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

void bsp_init(void);
void bsp_toggle_builtin_led(void);
void bsp_set_builtin_led(on_off_t led_state);
void bsp_spin_delay(size_t iter);
void bsp_error_trap(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_H */
