#ifndef WS2812_H_
#define WS2812_H_

#include <stdint.h>


// Configuration - SET THESE!
#define OUTPUT_PIN  (0x80)  // Set to whatever UCB0SIMO is on your processor (Px.7 here)
#define NUM_LEDS    (207)    // NUMBER OF LEDS IN YOUR STRIP


// Transmit codes
#define HIGH_CODE   (0xF0)      // b11110000
#define LOW_CODE    (0xC0)      // b11000000


typedef enum {
    COLOR_OFF,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    NUM_COLOR
} ColorMap;

// Configure processor to output to data strip
void initStrip(void);

// Send colors to the strip and show them. Disables interrupts while processing.
void showStrip(void);

// Set the color of a certain LED

void setLEDColor(uint32_t led, ColorMap color);

// Clear the color of all LEDs (make them black/off)
void clearStrip(void);

// Fill the strip with a solid color. This will update the strip.
void fillStrip(ColorMap color);

#endif
