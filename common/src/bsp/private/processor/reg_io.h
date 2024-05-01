/**
* @brief A CMSIS style register access header for the hardware peripherals on
* the ATmega328p
*/
#ifndef REG_IO_H
#define REG_IO_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PACKED __attribute__((packed))

#define IO__ volatile 
#define I___ const volatile
#define O___ volatile

typedef struct GpioPort
{
    IO__ u8_t  PIN;
    IO__ u8_t  DDR;
    IO__ u8_t  PORT;
} PACKED GpioPortTypeDef;

typedef struct Timer8Bit
{
    IO__ u8_t  TCCRA;
    IO__ u8_t  TCCRB;
    IO__ u8_t  TCNT;
    IO__ u8_t  OCRA;
    IO__ u8_t  OCRB;
} PACKED Timer8BitTypeDef;

typedef struct TimerRegIrq
{
    IO__ u8_t  TIFR;
         u8_t  reserved0[56];
    IO__ u8_t  TIMSK;
} PACKED TimerIrqRegTypeDef;

typedef struct Timer16Bit
{
    IO__ u8_t  TCCRA;
    IO__ u8_t  TCCRB;
    IO__ u8_t  TCCRC;
         u8_t  reserved0;
    IO__ u16_t TCNT;
    IO__ u16_t ICR;
    IO__ u16_t OCRA;
    IO__ u16_t OCRB;
} PACKED Timer16BitTypeDef;

typedef struct Usart 
{
    IO__ u8_t UCSRA;
    IO__ u8_t UCSRB;
    IO__ u8_t UCSRC;
         u8_t reserved0;
    IO__ u8_t UBRRL;
    IO__ u8_t UBRRH;
    IO__ u8_t UDR;
} PACKED UsartTypeDef;

#define GPIO_B      ((GpioPortTypeDef*)     (0x23))
#define GPIO_C      ((GpioPortTypeDef*)     (0x26))
#define GPIO_D      ((GpioPortTypeDef*)     (0x29))
#define TIM0_IRQ    ((TimerIrqRegTypeDef*)  (0x35))
#define TIM1_IRQ    ((TimerIrqRegTypeDef*)  (0x36))
#define TIM2_IRQ    ((TimerIrqRegTypeDef*)  (0x37))
#define TIM0        ((Timer8BitTypeDef*)    (0x44))
#define TIM1        ((Timer16BitTypeDef*)   (0x80))
#define TIM2        ((Timer8BitTypeDef*)    (0xB2))
#define USART0      ((UsartTypeDef*)        (0xC0))

#ifdef __cplusplus
}
#endif

#endif /* REG_IO_H */