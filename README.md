---
title: Readme
layout: template
filename: README
--- 

# EE-Emerge 2020: Four Face


FourFace is a capacitive touch, four player interactive game in the vein of the 80's electronic game, "Simon".  The basic premise of the game is a pattern of lights will emit for each player that they will then have to replicate. If they guess incorrectly, they are eliminated. With each round the patterns become more complex

## Table of Contents

1. [About the Project](#about-the-project)
1. [Getting Started](#getting-started)
    1. [Dependencies](#dependencies)
    1. [Installation](#installation)
1. [How to Get Help](#how-to-get-help)
1. [Authors](#authors)
1. [Acknowledgements](#acknowledgements)

# About the Project
FourFace is a project developed during the 2019/2020 academic year by undergraduates from EE-Emerge a group within the Department of Electrical and Computer Engineering at the University of California, Davis. FourFace is interactive memory game where four players compete against one another by replicating randomly generated patterns. As the rounds increase, the patterns become more difficult. Once a person fails to imitate the given pattern, they are eliminated. Last person to survive the remaining rounds is crowned the winner. At the core of FourFace's technology is CapSense technology and custom PCBs.


Features:
- CapSense Technology
- Custom PCB's
- 1-4 player game
- Adjustable Difficulty
- Portable
The mechanical design for the project is composed of a 15x15in cube with 9 acrylic tiles that face each player.

Our motivation for the project was to put a creative, more modern spin on a similar, but older game. We wanted to use our engineering skills to bring a faint idea into a reality by overseeing it through the whole product lifecycle. 

[Our Hackster.io website](https://www.hackster.io/fourface/fourface-1bf273)


**[Back to top](#table-of-contents)**

# Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

## Dependencies

### Required 
In order to build the firmware, [TI Code Composer](https://software-dl.ti.com/ccs/esd/documents/ccs_downloads.html) should be installed. When prompted select to add support for MSP430 and MSP432 microcontrollers. 

### Optional
Segger J-Link support can be added for MSP432, but is not required.

## Getting the Source

This project is [hosted on GitHub](hhttps://github.com/pmackle/EE-Emerge2020-FourFace). You can clone this project directly using this command:

```
git clone https://github.com/pmackle/EE-Emerge2020-FourFace
```

This will create a new root folder "EE-Emerge2020-FourFace" that will contain all projects for Four Face.

## Installation

1. Open Code Composer Studio
1. Create a new workspace 
    * File -> Switch Workspace -> Other
1. Browse to where the project repository was closed. 
1. Press launch
1. Import projects
    1. Project -> Import CCS Projects
    1. Press Browse
    1. Select root folder that was cloned.
    1. Press OK 
    1. Checkmark all projects
    1. Press Finish
1. Build each project one by one

**[Back to top](#table-of-contents)**

# How to Get Help

For help please contact one of the project [authors](#release-process).

**[Back to top](#table-of-contents)**

# Authors

* [Patrick Mackle](https://github.com/pmackle) - *Team Leader, Mechanical Design, Fabrication*
* [Anthony Pham](https://github.com/AnthonysPham) - *Mechanical Design, Fabrication*
* [Sophie Bolotin](https://github.com/sbolotin20) - *Website Design*
* [Noah Tarr](https://github.com/NoahTarr) - *Software Design*
* [Vanessa Liera](https://github.com/vpliera) - *Fabrication*
* [Victoria Liera](https://github.com/victorialiera) - *Fabrication*
* [Michelle Acoba](https://github.com/maacoba) - *Fabrication, Website Design*
* [Christian Gonzalez](https://github.com/christian-ee-emerge) - *Hardware & Mechanical Design, Fabrication*
* [Sean Alling](https://github.com/SeanAlling) - *Class TA*
* Andre Knoesen- *Professor*

**[Back to top](#table-of-contents)**

# Acknowledgments

UC Davis for giving us the opportunity to work together & Texas Instruments for sponsoring and allowing for FourFace to even become a reality.
A huge thank you to our professor Andre Knoessen and TA Sean Alling for their guidance in helping us stay on track with our goals and keep our sight on the big picture. 

**[Back to top](#table-of-contents)**
