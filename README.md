# RTCwakeUpComputer
> A low-cost, hardware-based solution for turning a computer on and off at designated times, centered around the DS3231 RTC.

I repurposed an old computer as a Linux FTP server to automatically back up all sorts of online projects. Because backups happen once a week during the night, it makes no sense to let the computer run 24/7. 
How lovely would it be if the computer automatically turned on, made the backup, and then turned off again? That's the reason why I invented RTCwakeUpComputer: a fully hardware-based solution to turn your computer on and off at specific times. It can be built within ~2 hours and should cost no more than 15 euros in materials.

> **Note**
> Modern computer systems can turn themselves on and off with all sorts of fancy tricks, such as Wake-On-Lan (WOL). Those tricks are almost always easier to implement, since they are based on software. Older computers, such as mine, do not have these luxuries.

![The final result, built into a computer.](/final.jpeg)
*Figure 1: The final result, built into a computer.*

## Table of contents


## Features

RTCwakeUpComputer is no ordinary clock. It features:

* A compact design that fits in box of ~10x5x3cm and can be built into a computer case;
* Fully customizable timeslots to turn your computer on and off;
* The ability to run besides existing methods of turning your system on/off, such as the power button;
* The ability to detect if a scheduled turn on/off was executed correctly, and to retry if not;
* The ability to keep accurate track of time, even if USB power is lost.

## Working principle

## Hardware design

These components are needed for the build:

* 1x Arduino Nano
* 1x DS3231 RTC with a backup battery
* 1x 5V relay
* 1x BS170 MOSFET, but any low-power N-channel MOSFET will do
* 1x diode, I used the 1N4007, but most regular diodes will work fine
* various wires
* various male headers
* soldering gear
* perfboard or any mounting solution of your choice
* USB cable to power and program the Arduino
* (optional) a box to put the circuit in. ~10x5x3cm will be okay.

## Software design






## Improvements & limitations



## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


## Acknowledgments



## Footnotes
<!---
gets rendered automatically
-->

Some footnotes (mostly nerdy additions):
