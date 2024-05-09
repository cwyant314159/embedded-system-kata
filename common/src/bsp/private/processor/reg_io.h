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

#define UART_UCSRA_MPCM_OFFSET      (0u)
#define UART_UCSRA_MPCM_MASK        (1u << UART_UCSRA_MPCM_OFFSET)
#define UART_UCSRA_U2X_OFFSET       (1u)
#define UART_UCSRA_U2X_MASK         (1u << UART_UCSRA_U2X_OFFSET)
#define UART_UCSRA_UPE_OFFSET       (2u)
#define UART_UCSRA_UPE_MASK         (1u << UART_UCSRA_UPE_OFFSET)
#define UART_UCSRA_DOR_OFFSET       (3u)
#define UART_UCSRA_DOR_MASK         (1u << UART_UCSRA_DOR_OFFSET)
#define UART_UCSRA_FE_OFFSET        (4u)
#define UART_UCSRA_FE_MASK          (1u << UART_UCSRA_FE_OFFSET)
#define UART_UCSRA_UDRE_OFFSET      (5u)
#define UART_UCSRA_UDRE_MASK        (1u << UART_UCSRA_UDRE_OFFSET)
#define UART_UCSRA_TXC_OFFSET       (6u)
#define UART_UCSRA_TXC_MASK         (1u << UART_UCSRA_TXC_OFFSET)
#define UART_UCSRA_RXC_OFFSET       (7u)
#define UART_UCSRA_RXC_MASK         (1u << UART_UCSRA_RXC_OFFSET)

#define UART_UCSRB_TXB8_OFFSET      (0u)
#define UART_UCSRB_TXB8_MASK        (1u << UART_UCSRB_TXB8_OFFSET)
#define UART_UCSRB_RXB8_OFFSET      (1u)
#define UART_UCSRB_RXB8_MASK        (1u << UART_UCSRB_RXB8_OFFSET)
#define UART_UCSRB_UCSZ2_OFFSET     (2u)
#define UART_UCSRB_UCSZ2_MASK       (1u << UART_UCSRB_UCSZ2_OFFSET)
#define UART_UCSRB_TXEN_OFFSET      (3u)
#define UART_UCSRB_TXEN_MASK        (1u << UART_UCSRB_TXEN_OFFSET)
#define UART_UCSRB_RXEN_OFFSET      (4u)
#define UART_UCSRB_RXEN_MASK        (1u << UART_UCSRB_RXEN_OFFSET)
#define UART_UCSRB_UDRIE_OFFSET     (5u)
#define UART_UCSRB_UDRIE_MASK       (1u << UART_UCSRB_UDRIE_OFFSET)
#define UART_UCSRB_TXCIE_OFFSET     (6u)
#define UART_UCSRB_TXCIE_MASK       (1u << UART_UCSRB_TXCIE_OFFSET)
#define UART_UCSRB_RXCIE_OFFSET     (7u)
#define UART_UCSRB_RXCIE_MASK       (1u << UART_UCSRB_RXCIE_OFFSET)

#define UART_UCSRC_UCPOL_OFFSET     (0u)
#define UART_UCSRC_UCPOL_MASK       (1u << UART_UCSRC_UCPOL_OFFSET)
#define UART_UCSRC_UCSZ0_OFFSET     (1u)
#define UART_UCSRC_UCSZ0_MASK       (1u << UART_UCSRC_UCSZ0_OFFSET)
#define UART_UCSRC_UCSZ1_OFFSET     (2u)
#define UART_UCSRC_UCSZ1_MASK       (1u << UART_UCSRC_UCSZ1_OFFSET)
#define UART_UCSRC_USBS_OFFSET      (3u)
#define UART_UCSRC_USBS_MASK        (1u << UART_UCSRC_USBS_OFFSET)
#define UART_UCSRC_UPM0_OFFSET      (4u)
#define UART_UCSRC_UPM0_MASK        (1u << UART_UCSRC_UPM0_OFFSET)
#define UART_UCSRC_UPM1_OFFSET      (5u)
#define UART_UCSRC_UPM1_MASK        (1u << UART_UCSRC_UPM1_OFFSET)
#define UART_UCSRC_UMSEL0_OFFSET    (6u)
#define UART_UCSRC_UMSEL0_MASK      (1u << UART_UCSRC_UMSEL0_OFFSET)
#define UART_UCSRC_UMSEL1_OFFSET    (7u)
#define UART_UCSRC_UMSEL1_MASK      (1u << UART_UCSRC_UMSEL1_OFFSET)

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