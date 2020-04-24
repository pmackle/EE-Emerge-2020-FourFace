# EE-Emerge 2020: Four Face


FourFace is a capacitive touch, four player interactive game in the vein of the 80's electronic game, "Simon".  The basic premise of the game is a pattern of lights will emit for each player that they will then have to replicate. If they guess incorrectly, they will be eliminated. With each round the patterns will become increasingly complex.

## Table of Contents

1. [About the Project](#about-the-project)
1. [Getting Started](#getting-started)
    1. [Dependencies](#dependencies)
    1. [Installation](#installation)
    1. [Building](#building)
1. [How to Get Help](#how-to-get-help)
1. [Authors](#authors)
1. [Acknowledgements](#acknowledgements)

# About the Project
FourFace is interactive memory game created by undergraduates from EE-Emerge, a group within the Department of Electrical and Computer 
Engineering at the University of California, Davis. The premise of this game is to compete against three other people and imitate the 
given pattern. As each round starts, the pattern becomes increasingly difficult. Once a person fails to imitate the given pattern, they 
will be eliminated. Last person to survive the remaining rounds is crowned the winner. Some highlights worth mentioning are CapSense 
technology, custom PCB's and mechanical design, and user friendliness.

Here you can provide more details about the project
What features does your project provide?
Features:
- CapSense Technology
- Custom PCB's
- 1-4 player game
- Difficulty adjustment
- Portable
The mechanical design for the project is composed of a 15x15in cube with 9 acrylic tiles that face each player.

Our motivation for the project was to put a creative, more modern spin on a similar, but older game. We wanted to use our engineering skills to bring a faint idea into a reality by overseeing it through the whole product lifecycle. 

[Our Hackster.io website](https://www.hackster.io/fourface/fourface-1bf273)

```
Show some example code to describe what your project does
Show some of your APIs
```

**[Back to top](#table-of-contents)**

# Getting Started

This section should provide instructions for other developers to

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

## Building

Instructions for how to build your project

```
Examples should be included
```

**[Back to top](#table-of-contents)**

# How to Get Help

For help please contact one of the project [authors](#release-process).

**[Back to top](#table-of-contents)**

# Authors

* [Patrick Mackle](https://github.com/pmackle) - Team Leader, Mechanical Design
* [Anthony Pham](https://github.com/AnthonysPham) - Assembly
* [Sophie Bolotin](https://github.com/sbolotin20) - Website Design
* [Noah Tarr](https://github.com/NoahTarr) - Software
* [Vanessa Liera](https://github.com/vpliera) - Assembly
* [Victoria Liera](https://github.com/victorialiera) - Assembly
* [Michelle Acoba](https://github.com/maacoba) - Assembly, Website Design
* [Christian Gonzalez](https://github.com/christian-ee-emerge) - Mechanical Design
* [Sean Alling](https://github.com/SeanAlling) - Class TA

**[Back to top](#table-of-contents)**

# Acknowledgments

UC Davis for giving us the opportunity to work together

Texas Instruments for sponsoring and allowing for FourFace to even become a reality

Professor Andre Knoesen for guiding us throught the big picture of our project

Sean Alling for making sure we did not drive ourselves insane by keeping us focused on achievable goals

**[Back to top](#table-of-contents)**
