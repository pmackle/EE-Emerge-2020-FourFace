---
title: Project
layout: template
filename: Project
--- 

# FourFace 
FourFace integrates capacitive touch sensing into a four-player interactive game in the vein of the 80's electronic game, "Simon". In the game, a pattern of lights emits and each player tries to replicate. If they guess incorrectly, the player is eliminated. The patterns become increasingly complex with new each round.   

![test!](https://github.com/pmackle/EE-Emerge-2020-FourFace/blob/master/Documentation/Photos/pic.png?raw=true)

## Mechanical Design

There are four adjacent sides on the cube that act as the interface for four players. CAD visualizes the framework, in order to create a rough and final prototype of the desired design.

Four interfaces interlock on each of the cube's sides with 3D printed hooks to hold the sides firmly together. Each face consists of nine acrylic squares functioning as the physical interface. Each frame is 15x15 inches. Within each frame, there are nine 4x4 inch acrylic panels with capacitive sensing technology, ultimately acting as a "touch screen."

Each frame has four layers, each having a specific mechanical function. The acrylic panel sits on top of the first layer and the first layer is directly above the second layer. The first and second layers act in conjunction with each other by creating a stable configuration for the panel, which is the layer above, to sit evenly on the first layer. Since the acrylic panel has the same dimensions as the first layer, without the second layer being slightly larger, the panels can be securely stacked. The next layer has a specific thickness of half an inch, which is the width of our LED strip. The LED strip runs along the square cutouts. The final layer allows wiring management.

## Software Design 

The project consists of four Texas Instruments' MSP430 and one MSP432. Universal Asynchronous Receiver/Transmitter (UART) Communication allows communication between the master (MSP432) and the slaves (MSP430). The master creates a random pattern and sends the pattern to the slaves. The slaves display the pattern for the player to imitate. This is a quick overview of how the algorithm works.

## Capacitive Touch

MSP430 consists of up to 16 capacitive touch pins embedded into its system. To build the capacitive touch system, the acrylic squares are laser cut to act as "touch screen buttons" for the users to press. The pattern emits onto that specific square. There is copper tape on the wood frame that makes contact with the acryllic sqaures for conductivity. The wires are soldered onto the copper tape connected to the PCBs to enable the capacitive touch. 

When a person makes contact (or comes near) the capacitive touch system, the capacitance changes. It is a parallel plate capacitor, with the acrylic being one plate and the finger being the other plate. Capacitance is inversely proportional to distance, so when a finger, for example, is near the system, the distance changes.

In the code, the number of charges and discharges to 5 volts is counted. When the capacitive touch system is untouched, there is a certain threshold value attributed to "untouched." However, the system indicates touch as the number of oscillations that is lower than the threshold value. The microcontroller recognizes this change and compares it to the pattern being emitted.
