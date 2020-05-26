---
title: Project
layout: template
filename: Project
--- 

# FourFace 
FourFace integrates capacitive touch sensing into a four-player interactive game in the vein of the 80's electronic game, "Simon". In the game, a pattern of lights emits and each player tries to replicate. If they guess incorrectly, the player is eliminated. The patterns become increasingly complex with new each round.   

![test!](https://github.com/pmackle/EE-Emerge-2020-FourFace/blob/master/Documentation/Photos/pic.png?raw=true)

## Mechanical Design

All of the mechanical design was done through Autodesk Fusion 360. Fourface is a cube that consists of 4 sides called "Faces", with 9, 4x4in acrylic tiles. The laser cut sides are held together by 3D printed corner pieces. One whole face is 15x15in.

Each frame has four layers, each with their own specific mechanical function. The first layer and second layer stack such that acrylic tiles can be placed securely on each face. The next layer has bigger holes so as allow for the LED strip to sit flush with the top layers. The final layer allows for cleaner wiring in a specific order.


![firstsecond!](https://github.com/pmackle/EE-Emerge-2020-FourFace/blob/master/Documentation/Photos/everything.png?raw=true)




## Software Design 

The project consists of four Texas Instruments' MSP430 and one MSP432 Launchpad. We use Universal Asynchronous Receiver/Transmitter (UART) Communication, allowing the communication between the master (MSP432) and slaves (MSP430). The master launchpad creates a random pattern and sends the pattern to the slaves. Once the pattern is received, the slaves display the pattern for the player to irepeat.

## Capacitive Touch

Texas Instruments' MSP430 has up to 16 capacitive touch pins embedded into its system. To build the capacitive touch system, we laser cut acrylic squares that acted as "touch screen buttons" for the users to press. Copper tape surrounds the tiles to allow for the system to recognize a change in capacitance. Wires are soldered onto the copper tape and connect to the PCB.

When a person makes contact (or comes near) the acrylic or copper tape, the capacitance changes. In summary, the finger and the system act as parallel plate capacitors. Our code counts the number of charges and discharges to 5 volts. When the capacitive touch system is untouched, there is a certain "untouched" threshold value. Because capacitance is inversely proportional to distance, we are able to use that change and tell the microcontroller that the tile has been touched. The microcontroller then compares the tile touched with the pattern that was emitted, to make sure the correct tile was touched.
