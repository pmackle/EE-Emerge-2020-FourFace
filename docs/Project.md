---
title: Project
layout: template
filename: Project
--- 

# FourFace 
FourFace integrates capacitive touch sensing into a four-player interactive game in the vein of the 80's electronic game, "Simon". In the game, a pattern of lights emits for each player that they have to replicate. If they guess incorrectly, the player is eliminated. With each round, the patterns become increasingly complex.

1. Mechanical Design

2. Software Design

3. Capacitive Touch

## Mechanical Design

Our vision of the mechanical design is a cube where the four adjacent sides act as the interface, allowing a maximum of four players. We use Autodesk Fusion 360 to visualize the framework, enabling us to create a rough and final prototype of the desired design.

Four interfaces interlock on each of the cube's sides with 3D printed hooks to hold the sides firmly together. Each panel consists of nine acrylic squares functioning as the physical interface. Each frame will have a dimension of 15x15 inches. Within each frame, there are nine 4x4 inch acrylic panels with capacitive sensing technology, ultimately acting as a "touch screen."

Each frame has four layers, each having a specific mechanical function. The acrylic panel sits on top of the first layer and the first layer is directly above the second layer. The first and second layers act in conjunction with each other by creating a stable configuration for the panel, which is the layer above, to sit evenly on the first layer. Since the acrylic panel has the same dimensions as the first layer, without the second layer being slightly larger, the panels can be securely stacked. The next layer has a specific thickness of half an inch, which is the width of our LED strip. The LED strip runs along the square cutouts. The final layer allows wiring management.

## Software Design 

The project consists of four Texas Instruments' MSP430 and one MSP432 Launchpad. We use Universal Asynchronous Receiver/Transmitter (UART) Communication, allowing the communication between the master launchpad (MSP432) and the slave launchpads (MSP430). The master launchpad creates a random pattern and sends the pattern to the slave launchpads. Once the pattern is received, the slave launchpads display the pattern for the player to imitate. This is a quick overview of how the algorithm works.

## Capacitive Touch

Texas Instruments' MSP430 has up to 16 capacitive touch pins embedded into its system. To build the capacitive touch system, we laser cut acrylic squares that acted as "touch screen buttons" for the users to press. The pattern emits onto that specific square. We also put copper tape on the wood frame for conductivity. This copper tape made contact with the acrylic squares. To enable this capacitive touch, our team soldered wires onto the copper tape and connected them to our PCBs.

When a person makes contact (or comes near) this capacitive touch system of the acrylic and copper tape, the capacitance changes. It is a parallel plate capacitor, with the acrylic being one plate and the finger being the other plate. Capacitance is inversely proportional to distance, so when a finger, for example, is near the system, the distance changes.

In our code, the number of charges and discharges to 5 volts is counted. When the capacitive touch system is untouched, there is a certain threshold value attributed that denotes being untouched. However, the system indicates touch as the number of oscillations is lower than the threshold value. The microcontroller recognizes this change and compares it to the pattern being emitted.
