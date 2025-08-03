#ifndef UART_H
#define UART_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void uart_init(void);
int uart_data_available(void);
uint8_t uart_read(void);
int uart_write(uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* UART_H */