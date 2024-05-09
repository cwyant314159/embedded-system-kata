#include "bsp/private/uart/uart.h"

#include <avr/interrupt.h>
#include "bsp/private/processor/reg_io.h"
#include "types.h"

/* Baud rate configuration */
#define BAUD    (19200u)                    /* 19.2K */
#define UBRR    ((F_CPU/(8.0f * BAUD)) - 1) /* assuming U2X0 is set */

/* Ring buffer infrastructure */
#define BYTE_RING_MAX_SIZE  (128u)
#define PRIVATE_RING_SIZE   BYTE_RING_MAX_SIZE    /* set ring size */
#define PRIVATE_RING_VOLATILE_DECL                      /* byte rings in this module are volatile */
#include "utils/private_ring.h"

PRIVATE_RING_DECLARATIONS(ByteRing, u8_t)                /* create ring type of bytes */
PRIVATE_RING_DECLARE(static volatile ByteRing, rx_ring); /* bytes to read             */
PRIVATE_RING_DECLARE(static volatile ByteRing, tx_ring); /* bytes to write            */

/* Readability macros for private ring functions */
#define BYTE_RING_INIT(var_name)       PRIVATE_RING_INIT(ByteRing, var_name)
#define BYTE_RING_IS_EMPTY(var_name)   PRIVATE_RING_IS_EMPTY(ByteRing, var_name)
#define BYTE_RING_IS_FULL(var_name)    PRIVATE_RING_IS_FULL(ByteRing, var_name)
#define BYTE_RING_PUSH(var_name, data) PRIVATE_RING_PUSH(ByteRing, var_name, data)
#define BYTE_RING_POP(var_name)        PRIVATE_RING_POP(ByteRing, var_name)
#define BYTE_RING_PEEK(var_name)       PRIVATE_RING_PEEK(ByteRing, var_name)

/**
 * @brief Initialize the UART hardware driver.
 */
void uart_init(void)
{
    u16_t ubrr_val; /* properly typed UBRR register value */
    u16_t ubrr_hi;  /* UBRR register high bits */
    u16_t ubrr_lo;  /* UBRR register low bits  */

    /* The baurd rate settings are split across 2 registers: high and low. */
    ubrr_val = (u16_t)UBRR;
    ubrr_hi  = (ubrr_val >> 8) & 0x0F;
    ubrr_lo  = (ubrr_val >> 0) & 0xFF;

    USART0->UBRRH = (u8_t)ubrr_hi;
    USART0->UBRRL = (u8_t)ubrr_lo;

    /* hard disable the UART */
    USART0->UCSRB = 0;
    USART0->UCSRA = 0;

    /*  Configure the control registers

        CSR A
        7. read-only (0)
        6. transmit complete clear on write (0)
        5. read-only (0)
        4. read-only (0)
        3. read-only (0)
        2. read-only (0)
        1. transmission speed doubler enabled (1)
        0. multi-processor communication disabled (0)

        CSRB
        7. disable RX complete interrupt (0)
        6. disable TX complete interrupt (0)
        5. disable UART data register empty interrupt (0)
        4. enable the RX (1)
        3. disable the TX (0)
        2. clear character size bit 2 (see CSRC for bits 0 - 1) (0)
        1. read-only (0)
        0. unused transmit data bit (0)

        CSRC
        7-6. UMSEL set to aynchronous UART (0)
        5-4. UPM set to disable parity checks (0)
        3.   1 stop bit (0)
        2-1. configure character size to 8-bit (b11)
        0.   unused synchronous only bit (0)

        NOTE: CSRB is configured last to allow the TX/RX enable to occurr after
              all other configuration.
    */
    USART0->UCSRA = UART_UCSRA_U2X_MASK;
    USART0->UCSRC = UART_UCSRC_UCSZ1_MASK | UART_UCSRC_UCSZ0_MASK;
    USART0->UCSRB = UART_UCSRB_RXCIE_MASK | UART_UCSRB_RXEN_MASK | UART_UCSRB_TXEN_MASK;

    /* Initialize byte rings */
    BYTE_RING_INIT(rx_ring);
    BYTE_RING_INIT(tx_ring);
}

/**
 * @brief Check if the driver's receiver has data bytes.
 *
 * @retval E_TRUE  - the driver's buffer has data bytes to read
 * @retval E_FALSE - the driver has no data to read
 */
bool_t uart_data_available(void)
{
    bool_t available;

    if (E_TRUE == BYTE_RING_IS_EMPTY(rx_ring)) {
        available = E_FALSE;
    } else {
        available = E_TRUE;
    }

    return available;
}

/**
 * @brief Read a byte from the driver's buffer
 *
 * @return A byte received over the UART or NULL ('\0') if there are no bytes
 * left.
 */
u8_t uart_read(void)
{
    u8_t byte;

    if (E_TRUE == uart_data_available()) {
        byte = BYTE_RING_POP(rx_ring);
    } else {
        byte = '\0';
    }

    return byte;
}

/**
 * @brief Write a byte to the driver's buffer
 *
 * @param[in] byte the data byte to transmit over the UART
 *
 * @retval E_TRUE  - byte successfully handled by driver
 * @retval E_FALSE - an error occurred during transmission
 */
bool_t uart_write(u8_t byte)
{
    bool_t result;

    /* If the ring is not full, add the byte to the buffer. */
    if (E_FALSE == BYTE_RING_IS_FULL(tx_ring)) {
        BYTE_RING_PUSH(tx_ring, byte);
        result = E_TRUE;
    } else {
        result = E_FALSE;
    }

    /* Regardless of the buffer state, we need to enable the transmitter to
       empty the byte we just added (or the back up of bytes preventing the
       ring push) */
    USART0->UCSRB |= UART_UCSRB_UDRIE_MASK;

    return result;
}

ISR(USART_RX_vect)
{
    u8_t data;

    /* Read the data regardless of the ring state to clear the interrupt */
    data = USART0->UDR;

    /* Only push to the ring if it is not full. */
    if (E_FALSE == BYTE_RING_IS_FULL(rx_ring)) {
        BYTE_RING_PUSH(rx_ring, data);
    }
}

ISR(USART_UDRE_vect)
{
    u8_t data;

    if (E_TRUE == BYTE_RING_IS_EMPTY(tx_ring)) {
        USART0->UCSRB &= ~UART_UCSRB_UDRIE_MASK;
    } else {
        data        = BYTE_RING_POP(tx_ring);
        USART0->UDR = data;
    }
}