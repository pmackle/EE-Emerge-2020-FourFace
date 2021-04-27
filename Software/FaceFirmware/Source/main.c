#include <msp430.h>
#include "ws2812.h"

#include <msp430g2553.h>
#include <stdint.h>
#include "structure.h"
#include "CTS_Layer.h"

#include "UART/uart.h"

//Functions
//LEDs
void setColorsShowStrip(uint8_t square, uint8_t color);
void receivePattern(void);
void sendPattern(void);
void resetLITSquares(void);
//Cap Touch Sensors
int runCapTouch();
void InitCapSenseButtons(void);
//Communications
void establishContact(void);
void UART_TX(char * tx_data);
void flashColor(ColorMap color);

//CHECK_BIT(temp, n - 1) where n is bits from right
#define CHECK_BIT(NUM,POS) ((NUM) & (0x1<<(POS)))

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
//  G | F | E  |  6 | 5 | 4  |  1.5 | 1.4 | 1.0
//  ---------  |  ---------  |  ---------------
//  H | C | D  |  7 | 2 | 3  |  2.2 | 2.1 | 1.6
//  ---------  |  ---------  |  ---------------
//  I | B | A  |  8 | 1 | 0  |  2.6 | 2.7 | 2.4

//2.4, 2.7, 2.1, 1.6, 1.0, 1.4, 1.5, 2.2, 2.6

uint8_t state = 0;
uint8_t litSquares[9] = {0};
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;  // Stop WDT
    if (CALBC1_16MHZ==0xFF)    // If calibration constant erased
    {
        while(1);              // do not load, trap CPU!!
    }
    DCOCTL = 0; // Select lowest DCOx and MODx
    // configure clock to 16 MHz
    BCSCTL1 = CALBC1_16MHZ;    // DCO = 16 MHz
    DCOCTL = CALDCO_16MHZ;
    
    /* Configure P1.1 and P1.2 for UART (USCI_A0) */
    P1SEL |= BIT1 + BIT2;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 |= BIT1 + BIT2;

    /* Global interrupt enable */
    __enable_interrupt();
    __bis_SR_register(GIE);

    uart_init();
    InitCapSenseButtons();
    initStrip();
    establishContact();
    setColorsShowStrip(SQUARES_C, COLOR_BLUE);

    //Ready up
    while(runCapTouch() != SQUARES_C) {
        __delay_cycles(300);
    }
    flashColor(COLOR_BLUE);
    uart_putchar('R');

    int UARTStreamChar;
    while (1)
    {
        UARTStreamChar = uart_getchar();
        if (UARTStreamChar != -1) {
            if (UARTStreamChar == 'R') {
                continue;
            }
            else if (UARTStreamChar == 'N') {
                receivePattern();
                sendPattern();
            }
            else if (UARTStreamChar == 'W') {
                fillStrip(COLOR_GREEN);
                showStrip();
                while(1){};
            }
            else if (UARTStreamChar == 'L') {
                fillStrip(COLOR_RED);
                showStrip();
                while(1){};
            }
        }
    }
}


void setColorsShowStrip(uint8_t square, uint8_t colorNum)
{
    ColorMap color;
    switch(colorNum)
    {
    case 0:
        color = COLOR_OFF;
        break;
    case 1:
        color = COLOR_RED;
        break;
    case 2:
        color = COLOR_GREEN;
        break;
    case 3:
        color = COLOR_BLUE;
    }

    ColorMap currentColor = COLOR_OFF;
    //Toggle passed square
    litSquares[square] = !litSquares[square];
    int k = 0;
    for (i = NUM_SQUARES; i > 0; i--) {
        currentColor = COLOR_OFF;
        if (litSquares[i])
            currentColor = color;
        for (k = i * LEDS_PER_SQUARE; k < (i + 1) * LEDS_PER_SQUARE; k++) 
            setLEDColor(k, currentColor);
    }
    showStrip();
}


void receivePattern(void)
{
    resetLITSquares();

    int square = -1;
    int color = -1;
    int state = 0;
    int UARTStreamChar = uart_getchar();
    do
    {
        if(UARTStreamChar != -1)
        {
            if(state == 0)
            {
                square = UARTStreamChar  - '0';
                state = 1;
            }
            else if(state == 1)
            {
                color = UARTStreamChar  - '0';
                state = 0;
                setColorsShowStrip(square, color);
                for (i = 1; i > 0; i--) {
                    __delay_cycles(10 * DELAY_100ms);
                }
                uart_putchar('D');
            }
        }
        UARTStreamChar = uart_getchar();
    } while (UARTStreamChar != 'E');
    flashColor(COLOR_BLUE);
    uart_putchar('D');
}


void sendPattern(void) {
    resetLITSquares();

    int capTouchValue = -1;
    int UARTStreamChar = -1;
    uint8_t playerLoses = 0;
    uint8_t square = 0;
    uint8_t color = 0;
    while(1) {
        capTouchValue = runCapTouch();
        if (capTouchValue != 0) {
            uart_putchar('P');
            uart_putchar(capTouchValue);

            while(1)
            {
                UARTStreamChar = uart_getchar();
                if(UARTStreamChar != -1)
                {
                    if (state == 0)
                    {
                        square = UARTStreamChar  - '0';
                        state = 1;
                    }
                    else if (state == 1)
                    {
                        color = UARTStreamChar  - '0';
                        state = 0;
                        setColorsShowStrip(square, color);
                        if (color == COLOR_RED) {
                            playerLoses = 1;
                        }
                        break;
                    }
                }
            }
        }
        if (uart_getchar() == 'E')
            break;
    }
    if (!playerLoses) {
        flashColor(COLOR_GREEN);
    }
    uart_putchar('D');
}


void resetLITSquares(void)
{
    for (i = 9; i > 0; i--) {
        litSquares[i] = 0;
    }
}


int runCapTouch()
{
    int result = 0;
    if (TI_CAPT_Button(&buttonSensor0))
        result = SQUARES_A;
    else if (TI_CAPT_Button(&buttonSensor1))
        result = SQUARES_B;
    else if (TI_CAPT_Button(&buttonSensor2))
        result = SQUARES_C;
    else if (TI_CAPT_Button(&buttonSensor3))
        result = SQUARES_D;
    else if (TI_CAPT_Button(&buttonSensor4))
        result = SQUARES_E;
    else if (TI_CAPT_Button(&buttonSensor5))
        result = SQUARES_F;
    else if (TI_CAPT_Button(&buttonSensor6))
        result = SQUARES_G;
    else if (TI_CAPT_Button(&buttonSensor7))
        result = SQUARES_H;
    else if (TI_CAPT_Button(&buttonSensor8))
        result = SQUARES_I;
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


void establishContact(void)
{
    int byteReceived = 0;
    while (!byteReceived) {
        if (uart_getchar() == 'R') {
            byteReceived = 1;
            uart_putchar('R');
        }
    }
//    uart_clearStream();
}

void flashColor(ColorMap color)
{
//    clearStrip();
//    showStrip();

    for (i = 1; i > 0; i--) {
        __delay_cycles(5 * DELAY_100ms);
    }

    fillStrip(color);
    showStrip();

    for (i = 1; i > 0; i--) {
        __delay_cycles(10 * DELAY_100ms);
    }

    clearStrip();
    showStrip();
}
