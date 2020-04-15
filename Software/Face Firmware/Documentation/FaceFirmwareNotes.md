# Four Face Firmware Notes

## Slave 

- Implemented with a MSP430
    * Slave communicates to the master using UART. 
        - UART is used since to meet timing requirements for the neopixels, the SPI peripheral is used. On this MSP430 the hardware I2C and the SPI share the same pins.
        - Master sends two bytes of data to the slave
            * Byte 0: 0-8 which corresponds to the square led square. 
            * Byte 1: 0-3 which corresponds to the color that the square should be.
        - Slave sends the master nine bytes
            * Each byte represent one square and the current color of the square. 