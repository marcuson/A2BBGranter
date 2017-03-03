# A2BBGranter

This project is part of a bigger one, namely Angular 2 Beyond Browser (A2BB), and it is the software part of a granter of the A2BB system.

The projects which are part of A2BB are:
* [A2BBServer](https://github.com/marcuson/A2BBServer)
* [A2BBAdminApp](https://github.com/marcuson/A2BBAdminApp)
* [A2BBUserSPA](https://github.com/marcuson/A2BBUserSPA)
* [A2BBMobileApp](https://github.com/marcuson/A2BBMobileApp)
* [A2BBGranter](https://github.com/marcuson/A2BBGranter)

and they have been developed as a demo to show the potentials of Angular2 - ASP.NET Core - Arduino technologies during a TechItalian meetup
(see [event here](https://www.meetup.com/TechItaliaTuscany/events/237721715)).

For an A2BB system description, please see [A2BBServer](https://github.com/marcuson/A2BBServer).

If you want to know more about TechItalians, see [TechItalia Tuscany MeetUp page](https://www.meetup.com/it-IT/TechItaliaTuscany/).

This project was generated with [PlatformIO](http://platformio.org/).

## Prerequisites

You must have a PlatformIO environment to build this project. Please see the [get started guide](http://platformio.org/get-started) of the
official documentation to see how to install PlatformIO on your PC.

Hardware parts needed are:
* 1 Arduino Mega 2560 or compatible
* 1 kOhm and 2 kOhm resistors (3 each)
* 2 voltage regulators from 9 V to 3.3 V
* 1 ESP8266 WiFi module
* 1 HC-05 BT module
* 1 ST7735 TFT 1.8'' screen
* jumper wires

## Hardware setup

Here an image of the hardware connections you need to make before building the Arduino sketch:

![Hardware connections](/docs/HWSchema_bb.png)

## Build and upload sketch to Arduino

Connect Arduino to PC using a male type-B to male type-A USB cable, then use the PlatformIO upload button to build and upload the sketch on Arduino.
