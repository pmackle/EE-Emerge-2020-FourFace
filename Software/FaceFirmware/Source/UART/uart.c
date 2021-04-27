/**
 * \file uart.c
 * \author Chris Karaplis
 * \brief MSP430 hardware UART driver
 *
 * Copyright (c) 2015, simplyembedded.org
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "uart.h"
#include "defines.h"
#include "ring_buffer.h"
#include <stdint.h>
#include <stddef.h>
#include <msp430.h>

struct baud_value
{
    uint32_t baud;
    uint16_t UCAxBR0;
    uint16_t UCAxBR1;
    uint16_t UCAxMCTL;
};

/* RX ring bufer */
static rbd_t _rbd;
static char _rbmem[8];

/**
 * \brief Initialize the UART peripheral
 * \param[in] config - the UART configuration
 * \return 0 on success, -1 otherwise
 */
void uart_init(void)
{
    const unsigned long MCLK_HZ = 16000000;          // SMCLK frequency in Hz
    const unsigned BPS = 9600;                       // ASYNC serial baud rate
    const unsigned long baud_rate_20_bits = (MCLK_HZ + (BPS >> 1)) / BPS; // Bit rate divisor

    // Configure USCI UART for 2400
    UCA0CTL1 = UCSWRST;                             // Hold USCI in reset to allow configuration
    UCA0CTL0 = 0;                                   // No parity, LSB first, 8 bits, one stop bit, UART (async)
    UCA0BR1 = (baud_rate_20_bits >> 12) & 0xFF;     // High byte of whole divisor
    UCA0BR0 = (baud_rate_20_bits >> 4) & 0xFF;      // Low byte of whole divisor
    UCA0MCTL = ((baud_rate_20_bits << 4) & 0xF0) | UCOS16; // Fractional divisor, over sampling mode
    UCA0CTL1 = UCSSEL_2;                            // Use SMCLK for bit rate generator, then release reset

    rb_attr_t attr = {sizeof(_rbmem[0]), ARRAY_SIZE(_rbmem), _rbmem};

    /* Initialize the ring buffer */
    if (ring_buffer_init(&_rbd, &attr) == 0) {
        /* Enable the USCI peripheral (take it out of reset) */
        UCA0CTL1 &= ~UCSWRST;

        /* Enable rx interrupts */
        IE2 |= UCA0RXIE;
    }
}

/**
 * \brief Read a character from UART
 * \return the character read on success, -1 if nothing was read
 */
int uart_getchar(void)
{
    int retval = -1;
    char c = -1;
    
    if (ring_buffer_get(_rbd, &c) == 0) {
        retval = (int) c;
    }

    return retval;
}


//Clear UART stream of characters
//Empties ring buffer
void uart_clearStream(void)
{
    while (uart_getchar() != -1) {}
}

/**
 * \brief Write a character to UART
 * \param[in] c - the character to write
 * \return 0 on sucess, -1 otherwise
 */
int uart_putchar(int c)
{
    /* Wait for the transmit buffer to be ready */
    while (!(IFG2 & UCA0TXIFG));

    /* Transmit data */
    UCA0TXBUF = (char) c;
    
    return 0;
}

/**
 * \brief Write a string to UART
 * \return 0 on sucesss, -1 otherwise
 */
int uart_puts(const char *str)
{
    int status = -1;

    if (str != NULL) {
        status = 0;

        while (*str != '\0') {
            /* Wait for the transmit buffer to be ready */
            while (!(IFG2 & UCA0TXIFG));

            /* Transmit data */
            UCA0TXBUF = *str; 

            /*  If there is a line-feed, add a carriage return */
            if (*str == '\n') {
                /* Wait for the transmit buffer to be ready */
                while (!(IFG2 & UCA0TXIFG));
                UCA0TXBUF = '\r';
            }

            str++;
        }
    }

    return status;
}

__attribute__((interrupt(USCIAB0RX_VECTOR))) void rx_isr(void)
{
    if (IFG2 & UCA0RXIFG) {
        const char c = UCA0RXBUF;
        
        /* Clear the interrupt flag */
        IFG2 &= ~UCA0RXIFG;

        ring_buffer_put(_rbd, &c);
    }
}
