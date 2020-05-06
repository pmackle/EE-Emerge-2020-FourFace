
During the prototype phase, we used Texas Instruments MSP430 and MSP432 launchpads to test the capsense technology, the software of the
game, and any wirings we have in mind. Once we figured out what we wanted out of this project, we became designing our PCBs. We will 
continue to use the MSP432 launchpad as our master. The software we will be using to create our PCBs is CircuitMaker by Altium Circuit 
Maker.

Here is a little background information on why we decided to make custom PCBs. At first, we thought of having the nine LED strips 
connected to the face launchpad. Along with the nine capsense acylic panels, the numbers of connections needed became 18 wires plus a 
communication wire, bring the total to 19 wires. However, The MSP430 do not support this many connections. With this in mind, we created 
our PCBs. 

The main job of the face PCBs is to read whether the capsense is activated. If the PCB detects a change in capacitance, then the 
PCB will be able to communicate with the Master PCB through UART. 


START CHANGES
Add some images here of the hardware
END CHANGES