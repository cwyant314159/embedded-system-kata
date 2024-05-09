#ifndef UART_H
#define UART_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void uart_init(void);
bool_t uart_data_available(void);
u8_t uart_read(void);
bool_t uart_write(u8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* UART_H */