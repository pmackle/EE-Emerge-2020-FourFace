
#include "ws2812.h"

#include <msp430.h>


/*
    Number of leds corresponds to the amount of leds that should be used. 
    The divide by 8 + 1 makes the system use the least amount of bytes to 
    represent all of the LEDs. 
*/
#define LED_ARRAY_SIZE (NUM_LEDS/4 + 1)




// WS2812 takes GRB format
typedef struct {
   uint8_t green;
   uint8_t red;
   uint8_t blue;
} LED;

 /*
    Each LED can have one of 4 defined colors. Each 8 bits holds 4 leds. This is done to make the 
    ram consumption more efficient. Compared to the naive way of using 3 bytes per led this will use 
    1/4 byte per led, a 12 fold decrease in ram consumption. 
 */
static uint8_t leds[LED_ARRAY_SIZE];

/*
    Defines the color mapping between the 2 bit color and the represented color
    Declared static const so that this is placed in flash and not ram
*/
static const LED ColorMapValues[NUM_COLOR] =
{
    {0x00, 0x00, 0x00}, //COLOR_OFF
    {0xFF, 0x00, 0x00}, //COLOR_RED,
    {0x00, 0x00, 0xFF}, //COLOR_GREEN,
    {0x00, 0xFF, 0x00}  //COLOR_BLUE,
};

/*
    Declared static const so that this is placed in flash and not ram
*/
static const  uint8_t ledBitmasks[NUM_COLOR] =
{
    0x03, // 0b0000_0011 LED 0
    0x0C, // 0b0000_1100 LED 1
    0x30, // 0b0011_0000 LED 2,
    0xC0 // 0b1100_0000 LED 3,
};

// Initializes everything needed to use this library. This clears the strip.
void initStrip() 
{
    // configure output pin as SPI output
    P1SEL  |= OUTPUT_PIN;
    P1SEL2 |= OUTPUT_PIN;
    // 3-pin, MSB, 8-bit SPI master
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;
    // SMCLK source (16 MHz)
    UCB0CTL1 |= UCSSEL_2;
    // 16 MHz / 3 = .1875 us per bit
    UCB0BR0 = 3;
    UCB0BR1 = 0;
    // Initialize USCI state machine
    UCB0CTL1 &= ~UCSWRST;

    // clear the strip
    clearStrip();
}

// Sets the color of a certain LED (0 indexed)
void setLEDColor(uint32_t ledIndex, ColorMap color) 
{
    uint32_t index = ledIndex/4;
    /*
        4 leds per byte. Get index of the led in the byte
    */
    uint8_t offset = ledIndex%4;

    /*
        And out the 2 bits for the led from the byte

    */
    leds[index] = (leds[index] & ~ledBitmasks[offset]);
    /*
        Or in the color code for the led. Offset is 0->3,
        multiply by 2 to get the correct shift amount.

        color is a value from 0 to 3. 
    */
    leds[index] = (leds[index] | (color<<(2*offset)));
}

// Send colors to the strip and show them. Disables interrupts while processing.
void showStrip() 
{
    __bic_SR_register(GIE);
    uint8_t *rgb = 0;
    // send RGB color for every LED
    for (uint32_t i = 0; i < NUM_LEDS; i++) 
    {
        /*
            Create a pointer to the led color codes in memory. 
            take led index number and divide by 8.

            i/4 -> Gives which byte led occupies within leds array.
            i%4 -> Gives index within byte that led occupies.
            (2*(i%4)) -> Shifts the led value to the two LSBs.
            ((leds[i/4]>>(2*(i%4))) returns the color code for the led.
                The color code is in RGB format and will be sent out over the SPI bus to 
                the LEDs.


            This was reworked so that it sends the same 3 tuple out on the spi bus as before 
            but it first extracts the color code value from packed bytes. By doing this there is a
            restriction on the number of colors that can be used by the program but in return the 
            amount of leds that can be addressed stays constant. 


        */

        rgb = (uint8_t *)&ColorMapValues[((leds[i/4]>>(2*(i%4)))&0x03)]; // get GRB color for this LED

        // send green, then red, then blue
        for (uint32_t j = 0; j < 3; j++)
        {
            uint8_t mask = 0x80;    // b1000000
            // check each of the 8 bits
            while (mask != 0)
            {
                while (!(IFG2 & UCB0TXIFG))
                {
                       // wait to transmit
                }
                // most significant bit first
                if (rgb[j] & mask)
                {
                    // send 1
                    UCB0TXBUF = HIGH_CODE;
                }
                else
                {
                    // send 0
                    UCB0TXBUF = LOW_CODE;
                }
                mask >>= 1;  // check next bit
            }
        }
    }

    // send RES code for at least 50 us (800 cycles at 16 MHz)
    __delay_cycles(800);

    // enable interrupts
    __bis_SR_register(GIE);    

}


// Clear the color of all LEDs (make them black/off)
void clearStrip() 
{
    fillStrip(COLOR_OFF);  // black
}

// Fill the strip with a solid color. This will update the strip.
void fillStrip(ColorMap color) 
{
    for (uint32_t i = 0; i < NUM_LEDS; i++) 
    {
        // set all LEDs to specified color
        setLEDColor(i, color);  
    }
}
