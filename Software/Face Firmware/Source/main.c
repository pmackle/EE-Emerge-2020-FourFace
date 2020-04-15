#include <msp430.h>
#include "ws2812.h"

#include <stdint.h>


void gradualFill(uint32_t n, ColorMap);
void UART_TX(char * tx_data);
int uart_putchar(int c);
int uart_getchar(void);

#define BUTTON    BIT3
unsigned i = 0;

uint8_t square = 0;
uint8_t color = 0;
uint8_t flag = 0;
uint8_t trash = 0;
uint8_t state = 0;
const unsigned long MCLK_HZ = 16000000;          // SMCLK frequency in Hz
const unsigned BPS = 9600;                       // ASYNC serial baud rate
int main(void)
{
    const unsigned long baud_rate_20_bits = (MCLK_HZ + (BPS >> 1)) / BPS; // Bit rate divisor
    WDTCTL = WDTPW + WDTHOLD;  // Stop WDT
    if (CALBC1_16MHZ==0xFF)    // If calibration constant erased
    {
        while(1);              // do not load, trap CPU!!
    }
    DCOCTL = 0; // Select lowest DCOx and MODx
    // configure clock to 16 MHz
    // clk cycle = 62.5ns
    BCSCTL1 = CALBC1_16MHZ;    // DCO = 16 MHz
    DCOCTL = CALDCO_16MHZ;
    /* Configure P1.1 and P1.2 for UART (USCI_A0) */
    P1SEL |= BIT1 + BIT2;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= BIT1 + BIT2;


    // Configure USCI UART for 2400
       UCA0CTL1 = UCSWRST;                             // Hold USCI in reset to allow configuration
       UCA0CTL0 = 0;                                   // No parity, LSB first, 8 bits, one stop bit, UART (async)
       UCA0BR1 = (baud_rate_20_bits >> 12) & 0xFF;     // High byte of whole divisor
       UCA0BR0 = (baud_rate_20_bits >> 4) & 0xFF;      // Low byte of whole divisor
       UCA0MCTL = ((baud_rate_20_bits << 4) & 0xF0) | UCOS16; // Fractional divisor, over sampling mode
       UCA0CTL1 = UCSSEL_2;                            // Use SMCLK for bit rate generator, then release reset


    __bis_SR_register(GIE);


    initStrip();  // ***** HAVE YOU SET YOUR NUM_LEDS DEFINE IN WS2812.H? ******
    // set strip color red
    fillStrip(COLOR_GREEN);

    showStrip();
    gradualFill(NUM_LEDS, COLOR_RED);  // green
    gradualFill(NUM_LEDS, COLOR_BLUE);  // blue
    gradualFill(NUM_LEDS, COLOR_GREEN);  // magenta

    while (1)
    {
        
        if(uart_getchar() != -1)
        {
            if((state == 0) && !flag)
            {
                square = UCA0RXBUF  - '0';
                state = 1;
            }
            else if((state == 1) && !flag)
            {
                color = UCA0RXBUF  - '0';
                state = 0;
                flag = 1;
            }
            else
            {
                trash = UCA0RXBUF;
                // Do nothing. ignore byte

            }
        }

        // Nes message received
        if(flag)
        {
            switch(color)
            {
            case 0:
                gradualFill(NUM_LEDS, COLOR_GREEN);  // magenta
                break;
            case 1:
                gradualFill(NUM_LEDS, COLOR_RED);  // green
                break;
            case 2:
                gradualFill(NUM_LEDS, COLOR_BLUE);  // blue
                break;
            }
            // show the strip
            showStrip();
            flag = 0;
        }

    }
}



void gradualFill(uint32_t n, ColorMap color)
{

    for (uint32_t i = 0; i < n; i++)
    {        // n is number of LEDs
        setLEDColor(i, color);
        __delay_cycles(10000);       // lazy delay
    }
    showStrip();
}



void UART_TX(char * tx_data) // Define a function which accepts a character pointer to an array
{
    unsigned int i=0;
    while(tx_data[i]) // Increment through array, look for null pointer (0) at end of string
    {
        while ((UCA0STAT & UCBUSY)); // Wait if line TX/RX module is busy with data
        UCA0TXBUF = tx_data[i]; // Send out element i of tx_data array on UART bus
        i++; // Increment variable for array address
    }
}



int uart_putchar(int c)
{
    /* Wait for the transmit buffer to be ready */
    while (!(IFG2 & UCA0TXIFG));

    /* Transmit data */
    UCA0TXBUF = (char ) c;

    return 0;
}



int uart_getchar(void)
{
    int chr = -1;

    if (IFG2 & UCA0RXIFG) {
        chr = UCA0RXBUF;
    }

    return chr;
}

