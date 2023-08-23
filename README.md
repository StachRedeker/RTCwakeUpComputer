# RTCwakeUpComputer
> A low-cost, hardware-based solution for turning a computer on and off at designated times, centered around the DS3231 RTC.

I repurposed an old computer as a Linux FTP server to automatically back up all sorts of online projects. Because backups happen once a week during the night, it makes no sense to let the computer run 24/7. 
How lovely would it be if the computer automatically turned on, made the backup, and then turned off again? That's the reason why I invented RTCwakeUpComputer: a fully hardware-based solution to turn your computer on and off at specific times. It can be built within ~2 hours and should cost no more than 15 euros in materials.

> **Note**
> Modern computer systems can turn themselves on and off with all sorts of fancy tricks, such as Wake-On-Lan (WOL). Those tricks are almost always easier to implement, since they are based on software. Older computers, such as mine, do not have these luxuries.

![The final result, built into a computer.](/Pictures/finalproduct.jpg)
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

This project is centred around the DS3231. The DS3231 is a Real-Time Clock (RTC) module, meaning that it can keep time accurately. But how do we actually turn on a computer using such a thing? 

If you have ever built (or taken apart) a desktop PC, you might be familiar with the `POWER SWITCH` jumper on the motherboard (also known as one of the 'front panel connectors'). If the two `POWER SWITCH` pins are shorted, the computer turns on (or off). 
So, I use an Arduino Nano to control a relay. The relay briefly shorts the `POWER SWITCH` pins to turn the system on or off. And that is functionally everything you need to make a hardware-based computer switch.

But wait, there's more. How do we validate that the computer is indeed turned on? We are running this thing in the middle of the night, so 'just' checking in on it is not an option. 
For that, we abuse another pair of jumpers on the motherboard: the `POWER LED` pins. The `POWER LED` pins are used to control the status LED (if present in your computer case). If the computer is turned on, there will be a voltage on the `POWER LED` pins. If the computer is turned off, there won't be. I use the Arduino to sense if a voltage is present, and hence I'm able to validate if a power on/off request was successful.

Lastly, a question that one might have is: "Why bother with all of this? Why not slam one of those time-controllable outlets between your computer and the power socket? That is cheaper and less of a hassle."
True, but that also means a 'hard' shutdown. Hard shutdowns can cause all sorts of problems, such as hard-drive write failures. Which is exactly what I do not want happening with a server that is used for backups. Hence, I prefer a 'soft' shutdown. **RTCwakeUpComputer results in the same kind of shutdown as a human operator would cause.**[^HARDSHUTDOWN]

[^HARDSHUTDOWN]: Another disadvantage of just cutting the power is that there is no guarantee that the computer turns on if the power is restored, unless you specifically tell it to in your BIOS.

![The final result, built into a computer.](/Pictures/connectors.jpg)
*Figure 1: The final result, built into a computer.*


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

![The final result, built into a computer.](/Pictures/circuit.png)
*Figure 1: The final result, built into a computer.*

![The final result, built into a computer.](/Pictures/frontback.jpg)
*Figure 1: The final result, built into a computer.*



## Software design






![The final result, built into a computer.](/Pictures/hookedup.jpg)
*Figure 1: The final result, built into a computer.*


![The final result, built into a computer.](/Pictures/off.jpg)
*Figure 1: The final result, built into a computer.*


## Improvements & limitations



## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


## Acknowledgments



## Footnotes
<!---
gets rendered automatically
-->

Some footnotes (mostly nerdy additions):
