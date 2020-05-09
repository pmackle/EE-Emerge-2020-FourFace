#include <msp430.h>
#include "ws2812.h"

#include <msp430g2553.h>
#include <stdint.h>
#include "structure.h"
#include "CTS_Layer.h"


// void setColorsShowStrip(uint8_t square, ColorMap color);
void setColorsShowStrip(uint8_t square, ColorMap color);
void UART_TX(char * tx_data);
int uart_putchar(int c);
int uart_getchar(void);

// uint16_t runCapTouch(uint16_t prevResult);
uint16_t runCapTouch();
void InitCapSenseButtons(void);

#define LEDS_PER_SQUARE (NUM_LEDS/9)
#define DELAY_100ms (800000)
unsigned i = 0;


//Enums
typedef enum  ledStripPins
{
    SQUARES_A,
    SQUARES_B,
    SQUARES_C,
    SQUARES_D,
    SQUARES_E,
    SQUARES_F,
    SQUARES_G,
    SQUARES_H,
    SQUARES_I,
    NUM_SQUARES
} squareNames;

////////////////////////////////////////
//SQUARE NUMBER LAYOUT BASED ON WIRING
////////////////////////////////////////
//  G | F | E  |  6 | 5 | 4
//  ---------  |  ---------
//  H | C | D  |  7 | 2 | 3
//  ---------  |  ---------
//  I | B | A  |  8 | 1 | 0

uint8_t square = 0;
uint8_t color = 0;
uint8_t flag = 0;
uint8_t trash = 0;
uint8_t state = 0;
uint8_t litSquares[9] = {0};
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


    InitCapSenseButtons();
    initStrip();  // ***** HAVE YOU SET YOUR NUM_LEDS DEFINE IN WS2812.H? ******


    fillStrip(COLOR_BLUE);
    showStrip();
    int byteReceived = 0;
    while (!byteReceived) {
        if (uart_getchar() == 'A') {
            uart_putchar('A');
            byteReceived = 1;
             __delay_cycles(10 * DELAY_100ms);
             fillStrip(COLOR_OFF);
             showStrip();
        }
    }

    uint16_t capTouchValue = 0x0;
    while (1)
    {
        capTouchValue = runCapTouch();
        if (capTouchValue != 0x0) {
            fillStrip(COLOR_GREEN);
            showStrip();
            __delay_cycles(50 * DELAY_100ms);
        }
        
        //Receive 2 bytes from master designating square and color.
        //First it receives the square, loops again and receives the color.
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
                
                 switch(color)
                 {
                 case 0:
                     setColorsShowStrip(square, COLOR_OFF);
                     break;
                 case 1:
                     setColorsShowStrip(square, COLOR_RED);
                     break;
                 case 2:
                     setColorsShowStrip(square, COLOR_GREEN);
                     break;
                 default:
                     setColorsShowStrip(square, COLOR_BLUE);
                 }
                 flag = 0;
                 __delay_cycles(10 * DELAY_100ms);
                 //Send 'A' back to master signifying it is done displaying the square.
                 //The master will not continue sending data until it receives this char.
                 uart_putchar('A');
             }
         }

    }
}


// void setColorsShowStrip(uint16_t crntStates)
void setColorsShowStrip(uint8_t square, ColorMap color)
{
    clearStrip();
    ColorMap currentColor = COLOR_OFF;
    //Toggle passed square
    litSquares[square] = !litSquares[square];
    int k = 0;
    for (int i = 0; i < NUM_SQUARES; i++) {
        currentColor = COLOR_OFF;
        if (litSquares[i])
            currentColor = color;
        for (k = i * LEDS_PER_SQUARE; k < (i + 1) * LEDS_PER_SQUARE; k++) 
            setLEDColor(k, currentColor);
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


// uint16_t runCapTouch(uint16_t prevResult)
uint16_t runCapTouch()
{
    // uint16_t result = prevResult;
    uint16_t result = 0x0;
    if (TI_CAPT_Button(&buttonSensor0))
        result ^= (0x1 << SQUARES_A);
    if (TI_CAPT_Button(&buttonSensor1))
        result ^= (0x1 << SQUARES_B);
    if (TI_CAPT_Button(&buttonSensor2))
        result ^= (0x1 << SQUARES_C);
    if (TI_CAPT_Button(&buttonSensor3))
        result ^= (0x1 << SQUARES_D);
    if (TI_CAPT_Button(&buttonSensor4))
        result ^= (0x1 << SQUARES_E);
    if (TI_CAPT_Button(&buttonSensor5))
        result ^= (0x1 << SQUARES_F);
    if (TI_CAPT_Button(&buttonSensor6))
        result ^= (0x1 << SQUARES_G);
    if (TI_CAPT_Button(&buttonSensor7))
        result ^= (0x1 << SQUARES_H);
    if (TI_CAPT_Button(&buttonSensor8))
        result ^= (0x1 << SQUARES_I);
    return result;
}


void InitCapSenseButtons(void)
{
    // Init each of the cap sense sensor. 
    TI_CAPT_Init_Baseline(&buttonSensor0);       // Establish baseline
    TI_CAPT_Update_Baseline(&buttonSensor0, 10); // Average 10 baseline measurements
    TI_CAPT_Init_Baseline(&buttonSensor1);
    TI_CAPT_Update_Baseline(&buttonSensor1, 10);
    TI_CAPT_Init_Baseline(&buttonSensor2);
    TI_CAPT_Update_Baseline(&buttonSensor2, 10);
    TI_CAPT_Init_Baseline(&buttonSensor3);
    TI_CAPT_Update_Baseline(&buttonSensor3, 10);
    TI_CAPT_Init_Baseline(&buttonSensor4);
    TI_CAPT_Update_Baseline(&buttonSensor4, 10);
    TI_CAPT_Init_Baseline(&buttonSensor5);
    TI_CAPT_Update_Baseline(&buttonSensor5, 10);
    TI_CAPT_Init_Baseline(&buttonSensor6);
    TI_CAPT_Update_Baseline(&buttonSensor6, 10);
    TI_CAPT_Init_Baseline(&buttonSensor7);
    TI_CAPT_Update_Baseline(&buttonSensor7, 10);
    TI_CAPT_Init_Baseline(&buttonSensor8);
    TI_CAPT_Update_Baseline(&buttonSensor8, 10);
    TI_CAPT_Update_Tracking_Rate(TRIDOI_VSLOW | TRADOI_FAST);   // Set baseline update rate to VSLOW in direction of interest
}
